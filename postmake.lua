local innosetup = postmake.loadplugin("internal/innosetup")
local shellscript = postmake.loadplugin("internal/shellscript")

local InnoAppID = "{{1CA82BG1-B475-45F7-9FFE-6DF654047519}"

-- App Settings
postmake.appname = "app"
postmake.appversion = "0.0.1"
postmake.output = "./output/install"

postmake.appinstalldir = "~/.ucodegameengine"

-- Short Hands
local all = postmake.allconfig
--local unix = postmake.foros("unix")
--local winodows = postmake.foros("winodws")
--local linux = postmake.foros("linux")
--local macos = postmake.foros("macos")

--- Configs
local win = postmake.newconfig("windows", "x64")
local gnu = postmake.newconfig("linux", "x64")
local mac = postmake.newconfig("macos", "x64")

--- Add Your files

local winsmainprogram = postmake.installdir() .. "./" .. postmake.appname .. ".exe"

win.addfile("main.exe", winsmainprogram)
gnu.addfile("main", postmake.installdir() .. "./" .. postmake.appname)
mac.addfile("main_macos", postmake.installdir() .. "./" .. postmake.appname)

all.addfile("License.md", postmake.installdir() .. "./License.md")

---@type ShellScriptConfig
local shellsettings = {}

postmake.make(shellscript, { gnu, mac }, shellscript);

---@type InnoSetConfig
local innosettings = {
  Appid = InnoAppID,
  MyAppURL = "https://github.com/LostbBlizzard/UCodeGameEngine",
  LaunchProgram = winsmainprogram,
  LicenseFile = "License.md",
  SetupIconFile = "UCodeGameEngineLogo.ico"
}

postmake.make(innosetup, { win }, innosettings);
