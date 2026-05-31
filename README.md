# tcxTemplate

A starter template for building a **TrussC addon**. Click **"Use this template"** on
GitHub (or copy this folder), rename `tcxTemplate` → your addon name, and start
writing code.

> An *addon* is a reusable library that drops into a TrussC project. Users add it
> with one line in `addons.make` (or `trusscli addon add tcxYourAddon`).

---

## 1. Quick start

```bash
# 1. Create your repo from this template, then clone it into a TrussC install:
trusscli addon clone tcxYourAddon          # (after it's published — see §6)
# ...or during development, just put the folder at <TrussC>/addons/tcxYourAddon

# 2. Build & run the bundled example:
cd addons/tcxYourAddon/example-basic
trusscli update            # generates CMakePresets.json / IDE files
trusscli run               # build + launch
```

Then rename things:

| In this template | Rename to |
|------------------|-----------|
| repo / folder name `tcxTemplate` | `tcxYourAddon` |
| `src/tcxTemplate.{h,cpp}` | `src/tcxYourAddon.{h,cpp}` |
| class `tcx::Template` | `tcx::YourClass` |
| `addon.json` fields | your metadata |

---

## 2. Folder layout

```
tcxYourAddon/
├── addon.json              # registry metadata (see §4)
├── README.md
├── LICENSE
├── src/                    # ← your addon code (.h / .cpp / .mm / .m)
│   ├── tcxYourAddon.h
│   └── tcxYourAddon.cpp
├── example-basic/          # a runnable example app (ships in the repo)
│   ├── addons.make         # lists this addon
│   ├── CMakeLists.txt      # 3-line stub; calls trussc_app()
│   └── src/                # main.cpp + tcApp.{h,cpp}
├── libs/                   # (optional) bundled 3rd-party libs, auto-collected
└── CMakeLists.txt          # (optional) only for custom builds — see §5
```

**Naming conventions** (match the rest of TrussC):
- Repo / folder / library target: `tcxPascalCase` (e.g. `tcxOsc`).
- Public namespace: `tcx`. Users write `using namespace tcx;`, so keep names short.
- Files: `tcxXxx.h`, types `PascalCase`, functions `camelCase`.
- Colors are `0.0–1.0` floats, angles are **radians**, use `TAU` (= 2π) not `PI`.
- Setters return `*this` for chaining; log with `tc::logNotice/logWarning/logError`,
  not `cout` (stdout is reserved for the MCP channel).

---

## 3. How a project consumes your addon

A user lists it in their project's `addons.make`:

```
tcxYourAddon
```

…and TrussC's build does the rest. **By default there is no CMakeLists.txt in an
addon** — TrussC auto-collects everything:

- compiles `src/**` and `libs/*/src/**` (`.cpp .c .mm .m`)
- adds `src/`, `include/`, and `libs/*/include` to the include path
- links your addon against `TrussC`
- header-only addon (no sources)? It becomes an `INTERFACE` target automatically.

So for a plain C++ addon you write **zero build config**. Just put headers and
sources in `src/`.

---

## 4. `addon.json`

Metadata for the registry crawler, the website, and `trusscli`. Must be **valid
JSON — no comments** (the crawler pipes it through `jq`). An empty `{}` already
makes the repo a discoverable addon.

**To be useful, fill these five — that's the whole "minimum":**

```json
{
  "description": "One-line summary of what your addon does",
  "author": "Your Name <you@example.com>",
  "license": "MIT",
  "category": "utilities",
  "keywords": ["example", "template"]
}
```

| Field | What it does |
|-------|--------------|
| `description` | One-line summary in listings / `trusscli addon list`. |
| `author` | `"Name"` or `"Name <email>"`. |
| `license` | SPDX id (`"MIT"`, `"Apache-2.0"`…). Missing → shows "Unknown". |
| `category` | One value from the fixed set ([below](#category--pick-exactly-one)). Unknown/omitted → `"misc"`. |
| `keywords` | Free tags for search/filtering within a category. |

### Optional fields

Everything else is optional — add only what's relevant, roughly in this order of
how often it's actually used:

| Field | What it does |
|-------|--------------|
| `dependencies` | Other addons yours needs ([below](#dependencies--other-addons-yours-needs)). Comes up fairly often. |
| `screenshot` | Relative path (`"docs/preview.png"`) or absolute URL. Shown on the site. |
| `demo_url` | Link to a live demo (e.g. a TrussSketch page). |
| `version` | Self-reported version. If omitted, the crawler uses your latest git tag — preferred, so just tag releases. |
| `trussc_version` | TrussC version range you target, e.g. `">=0.5.0"`. Informational; least-used field. |
| `platforms` | Platforms you've **tested**: `"macos" "win" "linux" "web" "android" "ios"`. Omit unless verified — empty `[]` means "supports nothing". |
| `name` | Informational only; the registry always uses the repo name. Leave it out. |

### `category` — pick exactly one

```
3d  ai  algorithms  animation  bridges  computer-vision  game  graphics
gui  hardware  machine-learning  network  physics  sound  typography
utilities  video  web
```

Anything unknown/empty maps to `misc`. Don't write `"misc"` yourself. Use
`keywords` for finer sub-classification (e.g. `category: "graphics"` +
`keywords: ["shader", "post-fx"]`).

### `dependencies` — other addons yours needs

Plain string (latest from registry) or an object to pin a version:

```jsonc
"dependencies": [
  "tcxCurl",                                       // latest, from registry
  { "name": "tcxBox2d", "tag":    "v0.2.0" },      // pinned tag
  { "name": "tcxOsc",   "branch": "main" },        // pinned branch
  { "name": "tcxBaz",   "commit": "a1b2c3d" },     // pinned commit
  { "name": "tcxBar",   "url": "https://github.com/user/tcxBar" } // off-registry
]
```

`tag`/`branch`/`commit` are mutually exclusive (`commit` wins if you set more
than one). `trusscli addon clone` resolves the graph recursively and aborts on
cycles. (This is for **addon→addon** deps. For a third-party C/C++ *library*,
use `FetchContent` in a custom `CMakeLists.txt` instead — see §5.)

---

## 5. When you need a custom build (`CMakeLists.txt`)

Add a `CMakeLists.txt` at the addon root **only** for things auto-collection
can't do: FetchContent, custom flags, code generation, platform link libs.
When present, TrussC `add_subdirectory()`s it and you own the target fully.

This template includes a fully-commented **`CMakeLists.txt.example`** — rename it
to `CMakeLists.txt` and edit. The essentials:

```cmake
set(ADDON_NAME tcxYourAddon)               # must equal the folder name
file(GLOB ADDON_SOURCES src/*.cpp)
add_library(${ADDON_NAME} STATIC ${ADDON_SOURCES})
target_include_directories(${ADDON_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(${ADDON_NAME} PUBLIC TrussC)   # always link TrussC
```

Real examples in the framework: `tcxBox2d` (FetchContent), `tcxTls`
(FetchContent + `configure_file` codegen + per-platform libs).

### Bundling a 3rd-party library without CMake

Drop it under `libs/somelib/` with `src/` and/or `include/` subfolders — auto
collection picks those up with no CMakeLists at all. Add `libs/` to
`.gitignore` if you fetch it instead of vendoring.

### Shipping a runtime file next to the app (DLL / dylib / metallib / data)

Some addons need a file to sit beside the final executable at **runtime** — a
Windows sensor-SDK `.dll`, a macOS `.dylib`/framework, a compiled `.metallib`,
a data blob. From a custom `CMakeLists.txt`, register it with:

```cmake
tc_addon_bundle_file(<path> [MACOS_DEST <subdir>])
```

TrussC copies it to the right place per platform, into whatever app consumes
your addon — no need to know the app's target name:

- **macOS:** into `YourApp.app/Contents/<MACOS_DEST>` (default `Resources`; use
  `Frameworks` for dylibs/frameworks).
- **Windows / Linux:** next to the executable (so the loader finds the DLL / `.so`).

```cmake
# e.g. a sensor SDK DLL that must sit beside the .exe on Windows:
if(WIN32)
    tc_addon_bundle_file("${CMAKE_CURRENT_SOURCE_DIR}/libs/sensor/sensor.dll")
endif()
# e.g. a generated Metal library into the app's Resources:
tc_addon_bundle_file("${CMAKE_CURRENT_BINARY_DIR}/default.metallib" MACOS_DEST Resources)
```

Generated files work too (e.g. a `metallib` produced by `add_custom_command`) —
just `add_dependencies()` your addon target on the generator so the file exists
before the app's post-build copy runs. Real example: **`tcxSyphon`** compiles
Syphon's shaders to `default.metallib` and bundles it this way.

---

## 6. Platform-specific code

- **macOS / iOS (Objective-C++):** put `.mm` / `.m` files in `src/`. They're
  compiled on every platform, so wrap the platform-only code in
  `#ifdef __APPLE__` (or `TARGET_OS_*`). See `tcxIME` / `tcxVirtualCam`.
- **Windows / Linux:** same idea — guard with `#ifdef _WIN32` / `__linux__`,
  or link platform libs in a custom `CMakeLists.txt` (`if(WIN32) … endif()`).
- **Web (Emscripten):** pure C++ generally just works. Avoid blocking I/O.

### Android: Java, res, and the manifest

An addon can ship Android Java helpers and resources — TrussC aggregates them
into the consuming app's APK automatically:

```
tcxYourAddon/android/
├── java/        # *.java — compiled (javac → d8) and bundled into classes.dex
├── res/         # Android resources — passed to aapt via -S
└── manifest/    # *.xml — manifest fragments the addon REQUESTS (advisory)
```

The app/example may also ship its own `android/java`, `android/res`, and
`android/AndroidManifest.xml(.in)`. At build time TrussC collects the project's
dirs **plus every loaded addon's** dirs, so `tcxMidi/android/java/*.java` and
your app's own Java compile together. (Native `.so` code links normally on
Android regardless — this is only about the Java/`.dex` + res side.)

**Manifest — advisory, never auto-injected.** The `AndroidManifest.xml` belongs
to the **app** and stays fully under the app's control: ship
`android/AndroidManifest.xml` (copied as-is) or `android/AndroidManifest.xml.in`
(`@TC_APP_PACKAGE@` / `@TC_APP_LIB_NAME@` / `@TC_APP_HAS_CODE@` expand via
configure_file). The default `minSdkVersion` is 26 — bump it in your own
manifest if your addon needs a higher API (e.g. native AMidi needs 29).

There is no manifest merger. If your addon needs a `<uses-permission>`,
`<uses-feature>`, `<receiver>`, or `<service>`, drop those lines in
`android/manifest/*.xml`. At configure time TrussC gathers all addon fragments
into `build-android/REQUIRED_MANIFEST_SNIPPET.xml` and prints a notice — the
**user** then copies in only the parts they want (they may deliberately drop a
permission). So: ship the fragment AND mention it in your README. Keep
`<uses-feature>` entries `required="false"` so you never block install on
devices lacking the hardware.

---

## 7. Publishing your addon

No pull request needed — the registry discovers addons by GitHub **topic**:

1. Push your repo to GitHub, **public**, not archived.
2. Repo name must match `tcx[A-Z]…` (e.g. `tcxYourAddon`).
3. Add the topic **`trussc-addon`** (repo → About → ⚙ → Topics).
4. Keep `addon.json` at the repo root (its presence is the opt-in marker).
5. Tag releases (`git tag v0.1.0 && git push --tags`) — the crawler records the
   latest tag as the version.

A scheduled crawl at [`TrussC-org/trussc-addons`](https://github.com/TrussC-org/trussc-addons)
finds it within a day and regenerates the registry (`registry.json` on
`gh-pages`), which `trusscli` reads.

> **Don't add the `trussc-addon` topic to a copy of *this* template** unless it's
> a real, finished addon — otherwise the literal `tcxTemplate` would show up in
> everyone's registry.

---

## License

This template is MIT. Replace `LICENSE` with your own and update the `license`
field in `addon.json`.
