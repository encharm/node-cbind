{
  "targets": [
    {
      "target_name": "tov8_core",
      "sources": [
        "src/addon.cc"
      ],
      "include_dirs"  : [
          "<!(node -e \"require('nan')\" 2> /dev/null)",
          "<!(node -e \"require('./')('core.nid')\" 2> /dev/null)",
          "<!(node -e \"require('./')('tests/*.nid')\" 2> /dev/null)",
          "./"
      ],
      "cflags": ["-g", "-std=c++11"],
			"cflags_cc!": [ '-fno-exceptions' ]
    }
  ]
}