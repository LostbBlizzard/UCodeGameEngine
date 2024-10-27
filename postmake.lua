local function executeorexit(str)
  local exit = os.execute(str)

  if exit == nil then
    os.exit(1)
  end

  if not exit == true then
    os.exit(1)
  end
end

local innosetup = postmake.loadplugin("internal/innosetup")
local shellscript = postmake.loadplugin("internal/shellscript")

local InnoAppID = "{{1CA82BG1-B475-45F7-9FFE-6DF654047519}"

-- App Settings
postmake.appname = "UCodeGameEngine"
postmake.appversion = "0.0.1"
postmake.apppublisher = "LostbBlizzard"
postmake.output = "./Output/install"
postmake.appinstalldir = "~/.ucodegameengine"
postmake.applicensefile = "LICENSE.txt"

-- Short Hands
local all = postmake.allconfig

--- Configs
local win = postmake.newconfig("windows", "x64")
local gnu = postmake.newconfig("linux", "x64")
local mac = postmake.newconfig("macos", "x64")

--- Add Your files

local programdir = postmake.installdir() .. "bin/"
local unixmainprogram = programdir .. postmake.appname
local winsmainprogram = unixmainprogram .. ".exe"

local programbuilddir = "./Output/UCodeEditor/linux64/Debug/"

win.addxfile(programbuilddir .. "UCodeEditor", winsmainprogram)
gnu.addxfile(programbuilddir .. "UCodeEditor", unixmainprogram)
mac.addxfile(programbuilddir .. "UCodeEditor", unixmainprogram)

-- all.addfile(programbuilddir .. "UFiles.data", programdir .. "UFiles.data")

all.addfile(postmake.applicensefile, postmake.installdir() .. postmake.applicensefile)

local addpathflag = all.newflag("Add Path", true)

all.If(addpathflag).addpath(programdir)

executeorexit("cd ./UCodeGameEngineDoc && mdbook build --dest-dir ../Output/UCodeGameEngineDoc")
all.addfile("Output/UCodeGameEngineDoc/.**", postmake.installdir() .. "doc")

local installwebsite = "https://github.com/LostbBlizzard/UCodeGameEngine/releases/tag/Release-" .. postmake.appversion

---@type ShellScriptConfig
local shellsettings = {
  weburl = installwebsite,
  uploaddir = "./Output/upload/",
}

---@type InnoSetConfig
local innosettings = {
  AppId = InnoAppID,
  MyAppURL = "https://github.com/LostbBlizzard/UCodeGameEngine",
  LaunchProgram = winsmainprogram,
  SetupIconFile = "UCodeGameEngineLogo.ico",
  UninstallDelete = {
    programdir .. "imgui.ini"
  },

}

-- TODO missing line
-- Name: "{userdesktop}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon;

postmake.make(shellscript, { gnu, mac }, shellsettings);
postmake.make(innosetup, { win }, innosettings);
