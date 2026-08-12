# FSD

FSD is a C++ library and Python extension for reading and working with FSD
data.

We do not plan on developing FSD further as we now prefer to use cFSD to handle our games' static data, but, in the spirit of developer transparency we believe there is value in open sourcing this.

## Building

This project uses CMake and vcpkg.

```sh
cmake --preset x64-windows-release
cmake --build .cmake-build-x64-windows-release --config Release
```

Available presets and build options are defined in `CMakePresets.json` and
`CMakeLists.txt`.

## Testing

Tests are integrated with CTest.

```sh
ctest --test-dir .cmake-build-x64-windows-release -C Release --output-on-failure
```

## 🤝 Contributing

Contribution follows the standard GIT PR model.

By submitting a pull request or otherwise contributing to this project, you
agree to license your contribution under the MIT License, and you confirm that
you have the right to do so.

## 📄 License and Legal Notices

© 2026 CCP Games

This software is provided by CCP Games.

Trademark Notice: CCP Games is a trademark of CCP ehf.

This project is licensed under the [MIT License](LICENSE.md). Nothing in the
[MIT License](LICENSE.md) grants any rights to CCP Games' trademarks or game
content.
