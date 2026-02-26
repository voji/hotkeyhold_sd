import { action, KeyDownEvent, KeyUpEvent, SingletonAction, WillAppearEvent, DidReceiveSettingsEvent } from "@elgato/streamdeck";

import { createRequire } from 'module';
const require = createRequire(import.meta.url);

const winnapi = require("./addons/winnapi.node");

/**
 * Sends a key event to the operating system.
 */
@action({ UUID: "hu.voji.keyboard.sendkey" })
export class SendKey extends SingletonAction<SendKeySettings> {

	/**
	 * Listens for the {@link SingletonAction.onKeyDown} event which is emitted by Stream Deck when an action is pressed. Stream Deck provides various events for tracking interaction
	 * with devices including key down/up, dial rotations, and device connectivity, etc. When triggered, {@link ev} object contains information about the event including any payloads
	 * and action information where applicable. 
	 */
	override async onKeyDown(ev: KeyDownEvent<SendKeySettings>): Promise<void> {
		const { settings } = ev.payload;
		var shift = typeof settings.mod_shift === 'boolean' ? settings.mod_shift : false;
		var ctrl = typeof settings.mod_ctrl === 'boolean' ? settings.mod_ctrl : false;
		var alt = typeof settings.mod_alt === 'boolean' ? settings.mod_alt : false;
		var win = typeof settings.mod_win === 'boolean' ? settings.mod_win : false;
		winnapi.sendKeyDown(settings.hotkey, shift, ctrl, alt, win);
	}

	/**
	 * Listens for the {@link SingletonAction.onKeyUp} event which is emitted by Stream Deck when an action is released. Similar to {@link onKeyDown}, this method handles the key release event.
	 * The {@link ev} object contains information about the event including any payloads and action information where applicable.
	 */
	override async onKeyUp(ev: KeyUpEvent<SendKeySettings>): Promise<void> {
		const { settings } = ev.payload;
		var shift = typeof settings.mod_shift === 'boolean' ? settings.mod_shift : false;
		var ctrl = typeof settings.mod_ctrl === 'boolean' ? settings.mod_ctrl : false;
		var alt = typeof settings.mod_alt === 'boolean' ? settings.mod_alt : false;
		var win = typeof settings.mod_win === 'boolean' ? settings.mod_win : false;
		winnapi.sendKeyUp(settings.hotkey, shift, ctrl, alt, win);
	}

	/**
	 * Handle onAppear and update the button title (if necessary)
	 */
	override async onWillAppear(ev: WillAppearEvent<SendKeySettings>): Promise<void> {
		const { settings } = ev.payload;
		await this.updateTitle(ev, settings);
	}

	/**
	* Handles settings change and updates the button title (if necessary)
	*/
	override async onDidReceiveSettings(ev: DidReceiveSettingsEvent<SendKeySettings>): Promise<void> {
		const { settings } = ev.payload;
		await this.updateTitle(ev, settings);
	}

	/**
	 * Update the button title (if necessary)
	 */
	private async updateTitle(ev: any, settings: SendKeySettings): Promise<void> {
		const generatedTitle = this.generateTitleFromSettings(settings);
		await ev.action.setTitle(generatedTitle);
	}


	/**
	 * Generates a display string from the current settings
	  */
	private generateTitleFromSettings(settings: SendKeySettings): string {
		const modifiers = [];
		if (settings.mod_shift) modifiers.push("Shift");
		if (settings.mod_ctrl) modifiers.push("Ctrl");
		if (settings.mod_alt) modifiers.push("Alt");
		if (settings.mod_win) modifiers.push("Win");

		var modifierText = modifiers.length > 0 ? modifiers.join("+") : "";
		const hotkey = typeof settings?.hotkey === "string" ? settings.hotkey.trim() : "";
		if (modifierText.length > 0 && hotkey.length > 0) {
			modifierText += "+";
		}
		return `${modifierText}${hotkey}`;
	}

}

/**
 * Settings for {@link SendKey}.
 */
type SendKeySettings = {
	hotkey: string;
	mod_shift: boolean;
	mod_ctrl: boolean;
	mod_alt: boolean;
	mod_win: boolean;
};
