{
  "targets": [
    {
      "target_name": "winnapi",
      "sources": [ "win_napi.cpp" ],
      "include_dirs": ["<!(node -p \"require('node-addon-api').include\")"],
      "dependencies": ["<!(node -p \"require('node-addon-api').gyp\")"],
      "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ],
      "libraries": [ "user32.lib" ],
    }, 
    {
      "target_name": "copy_binary",
      "type":"none",
      "dependencies" : [ "winnapi" ],
      "copies":
      [
        {
            'destination': '<(module_root_dir)/../hu.voji.keyboard.hotkeyhold.sdPlugin/bin/addons',
            'files': ['<(module_root_dir)/build/Release/winnapi.node']
        }
      ]
    }
  ]
}
