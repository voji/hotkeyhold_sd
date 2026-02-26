#include "../node_modules/node-addon-api/napi.h"
#include <windows.h>
#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>

WORD getVirtualKeyFromChar(char c) {
    SHORT vk = VkKeyScanA(c);
    if (vk == -1) return 0;
    return LOBYTE(vk);
}

bool getBoolArg(const Napi::CallbackInfo& info, size_t index) {
    return info.Length() > index && info[index].IsBoolean() && info[index].As<Napi::Boolean>();
}

std::string trim(const std::string& value) {
    const size_t start = value.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    const size_t end = value.find_last_not_of(" \t\n\r");
    return value.substr(start, end - start + 1);
}

std::string toUpper(std::string value) {
    std::transform(
        value.begin(),
        value.end(),
        value.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); }
    );
    return value;
}

WORD getVirtualKeyFromString(const std::string& rawKey) {
    const std::string key = toUpper(trim(rawKey));
    if (key.empty()) return 0;

    if (key.size() > 1 && key[0] == 'F') {
        const std::string digits = key.substr(1);
        if (!digits.empty() && std::all_of(digits.begin(), digits.end(), [](unsigned char c) { return std::isdigit(c); })) {
            const int functionKey = std::stoi(digits);
            if (functionKey >= 1 && functionKey <= 24) {
                return static_cast<WORD>(VK_F1 + (functionKey - 1));
            }
        }
    }

    static const std::unordered_map<std::string, WORD> keyMap = {
        {"LEFT", VK_LEFT},
        {"RIGHT", VK_RIGHT},
        {"UP", VK_UP},
        {"DOWN", VK_DOWN},
        {"ENTER", VK_RETURN},
        {"RETURN", VK_RETURN},
        {"ESC", VK_ESCAPE},
        {"ESCAPE", VK_ESCAPE},
        {"TAB", VK_TAB},
        {"SPACE", VK_SPACE},
        {"BACKSPACE", VK_BACK},
        {"DELETE", VK_DELETE},
        {"DEL", VK_DELETE},
        {"HOME", VK_HOME},
        {"END", VK_END},
        {"PAGEUP", VK_PRIOR},
        {"PAGEDOWN", VK_NEXT},
        {"INSERT", VK_INSERT}
    };

    const auto it = keyMap.find(key);
    if (it != keyMap.end()) {
        return it->second;
    }

    if (key.size() == 1) {
        return getVirtualKeyFromChar(key[0]);
    }

    return 0;
}

void sendInput(bool down, WORD vk) {
    
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = down ? 0 : KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}

void handleSpecialKeys(bool down, const Napi::CallbackInfo& info) {
    bool modShift = getBoolArg(info, 1);
    if (modShift) sendInput(down, VK_SHIFT);
    bool modCtrl = getBoolArg(info, 2);
    if (modCtrl) sendInput(down, VK_CONTROL);
    bool modAlt = getBoolArg(info, 3);
    if (modAlt) sendInput(down, VK_MENU);
    bool modWin = getBoolArg(info, 4);
    if (modWin) sendInput(down, VK_LWIN);
}


void sendKey(bool down, const Napi::CallbackInfo& info) {
    if (down) {
        handleSpecialKeys(down, info);
    }
    if (info[0].IsString()) {
        std::string key = info[0].As<Napi::String>();
        WORD vk = getVirtualKeyFromString(key);
        if (vk > 0) {
            sendInput(down, vk);
        }
    }
    if (!down) {
        handleSpecialKeys(down, info);
    }
}

Napi::Value SendKeyDown(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();    
    sendKey(true, info);
    return env.Undefined();
}

Napi::Value SendKeyUp(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    sendKey(false, info);
    return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("sendKeyDown", Napi::Function::New(env, SendKeyDown));
    exports.Set("sendKeyUp", Napi::Function::New(env, SendKeyUp));
    return exports;
}

NODE_API_MODULE(key_sender, Init)
