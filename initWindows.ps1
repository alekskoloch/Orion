# =========================================== Admin step

if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    Write-Host "This script requires administrator privileges. Restarting with elevated permissions..." -ForegroundColor Yellow
    Start-Process powershell.exe -ArgumentList "-NoProfile -ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    exit
}

# =========================================== Git step

$GitCommand = Get-Command "git" -ErrorAction SilentlyContinue

if ($GitCommand) {
    Write-Host "Git is currently installed." -ForegroundColor Green
} else {
    Write-Host "Git is missing." -ForegroundColor Yellow
    $UserInput = Read-Host "Do you want install Git? (Y/N)"

    if ($UserInput -eq 'Y') {
        Write-Host "Starting Git installation process..." -ForegroundColor Cyan
        
        winget install --id Git.Git -e --source winget --silent --accept-package-agreements --accept-source-agreements
        
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
        
        if (Get-Command "git" -ErrorAction SilentlyContinue) {
            Write-Host "Installation finished and Git is now available!" -ForegroundColor Green
        } else {
            Write-Host "Git was installed, but you might need to restart the terminal to use it." -ForegroundColor Yellow
        }
    } else {
        Write-Host "Installation skipped by user." -ForegroundColor Gray
    }
}

# =========================================== CMake step

$CMakeCommand = Get-Command "cmake" -ErrorAction SilentlyContinue

if ($CMakeCommand) {
    Write-Host "CMake is currently installed." -ForegroundColor Green
} else {
    Write-Host "CMake is missing." -ForegroundColor Yellow
    $UserInput = Read-Host "Do you want install CMake? (Y/N)"

    if ($UserInput -eq 'Y') {
        Write-Host "Starting CMake installation process..." -ForegroundColor Cyan
        
        winget install --id Kitware.CMake -e --source winget --silent --accept-package-agreements --accept-source-agreements
        
        $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
        
        if (Get-Command "cmake" -ErrorAction SilentlyContinue) {
            Write-Host "Installation finished and CMake is now available!" -ForegroundColor Green
        } else {
            Write-Host "CMake was installed, but you might need to restart the terminal to use it." -ForegroundColor Yellow
        }
    } else {
        Write-Host "Installation skipped by user." -ForegroundColor Gray
    }
}

# =========================================== LLVM step

$Tools = @{
    "clang"        = "Compiler"
    "clang-tidy"   = "Static Analysis"
    "clang-format" = "Formatter"
    "lld"          = "Linker"
    "lldb"          = "Debugger"
}

function Get-MissingTools {
    $missing = @()
    foreach ($Tool in $Tools.Keys) {
        if (-not (Get-Command $Tool -ErrorAction SilentlyContinue)) {
            $missing += "$Tool ($($Tools[$Tool]))"
        }
    }
    return $missing
}

$DefaultLlvmBin = "C:\Program Files\LLVM\bin"
$MissingBefore = Get-MissingTools

if ($MissingBefore.Count -eq 0) {
    Write-Host "Full LLVM toolchain is already installed and in PATH." -ForegroundColor Green
} else {
    Write-Host "LLVM tools are missing from PATH." -ForegroundColor Yellow

    if (Test-Path "$DefaultLlvmBin\clang.exe") {
        Write-Host "Detected LLVM files at $DefaultLlvmBin. Fixing PATH..." -ForegroundColor Cyan
        
        $CurrentPath = [System.Environment]::GetEnvironmentVariable("Path", "Machine")
        
        if ($CurrentPath -notlike "*LLVM*") {
            $NewPath = $CurrentPath + ";" + $DefaultLlvmBin
            [System.Environment]::SetEnvironmentVariable("Path", $NewPath, "Machine")
            Write-Host "PATH updated in System Registry." -ForegroundColor Green
        }
        
        $env:Path += ";$DefaultLlvmBin"
    } 
    else {
        Write-Host "LLVM not found on disk. Installing via Winget..." -ForegroundColor Cyan
        winget install --id LLVM.LLVM -e --source winget --silent --accept-package-agreements --accept-source-agreements --override "/S /Add-Path=1 /AllUsers"
        
        if (Test-Path $DefaultLlvmBin) { $env:Path += ";$DefaultLlvmBin" }
    }

    $MissingAfter = Get-MissingTools
    if ($MissingAfter.Count -eq 0) {
        Write-Host "LLVM toolchain is now functional!" -ForegroundColor Green
    } else {
        Write-Host "ERROR: Still cannot find LLVM tools. You may need to restart the PC." -ForegroundColor Red
        foreach ($M in $MissingAfter) { Write-Host " - Missing: $M" -ForegroundColor Red }
    }
}

# =========================================== Boost step

$EnvDir = Join-Path $PSScriptRoot "env"
$BoostVerFile = Join-Path $EnvDir "boost_version.txt"

if (-not (Test-Path $BoostVerFile)) {
    Write-Host "Warning: $BoostVerFile not found. Creating default (1.90.0)..." -ForegroundColor Yellow
    if (-not (Test-Path $EnvDir)) { New-Item -ItemType Directory -Path $EnvDir | Out-Null }
    Set-Content -Path $BoostVerFile -Value "1.90.0"
}

$BoostVersion = (Get-Content $BoostVerFile).Trim()
$BoostVerUnderscore = $BoostVersion -replace '\.', '_'
$BoostDirName = "boost_$BoostVerUnderscore"

$BoostRootBase = "C:\Boost"
$TargetBoostPath = Join-Path $BoostRootBase $BoostDirName

$BoostStageLib = Join-Path $TargetBoostPath "stage\lib"

if (Test-Path $BoostStageLib) {
    Write-Host "Boost $BoostVersion is already installed and compiled at $TargetBoostPath" -ForegroundColor Green
} else {
    Write-Host "Boost $BoostVersion libraries are missing." -ForegroundColor Yellow
    $UserInput = Read-Host "Do you want to download and compile Boost $BoostVersion? (This will take time!) (Y/N)"

    if ($UserInput -eq 'Y') {
        if (-not (Test-Path $BoostRootBase)) { New-Item -ItemType Directory -Path $BoostRootBase | Out-Null }
        
        $BoostUrl = "https://archives.boost.io/release/$BoostVersion/source/$BoostDirName.zip"
        $ZipPath = Join-Path $env:TEMP "$BoostDirName.zip"

        Write-Host "Downloading Boost from $BoostUrl..." -ForegroundColor Cyan
        try {
            Invoke-WebRequest -Uri $BoostUrl -OutFile $ZipPath -ErrorAction Stop
        } catch {
            Write-Host "Error downloading Boost. Check internet connection or version number in boost_version.txt." -ForegroundColor Red
            Write-Host "Failed URL: $BoostUrl" -ForegroundColor Red
            exit
        }

        Write-Host "Extracting Boost (please wait)..." -ForegroundColor Cyan
        if (Test-Path $TargetBoostPath) { Remove-Item $TargetBoostPath -Recurse -Force }
        
        Expand-Archive -Path $ZipPath -DestinationPath $BoostRootBase -Force
        Remove-Item $ZipPath -Force

        Write-Host "Compiling Boost libraries (Bootstrap + B2)..." -ForegroundColor Cyan
        Set-Location $TargetBoostPath
        
        if (Test-Path "bootstrap.bat") {
            cmd /c "bootstrap.bat"
            
            if ($LASTEXITCODE -eq 0) {
                Write-Host "Bootstrap successful. Starting build..." -ForegroundColor Cyan
                cmd /c "b2.exe link=static threading=multi runtime-link=shared address-model=64 variant=debug,release stage"
            } else {
                Write-Host "Error: Bootstrap failed." -ForegroundColor Red
                exit
            }
        } else {
            Write-Host "Error: bootstrap.bat not found in $TargetBoostPath" -ForegroundColor Red
            exit
        }

        Write-Host "Boost compilation finished!" -ForegroundColor Green
    } else {
        Write-Host "Boost installation skipped." -ForegroundColor Gray
    }
}

$CurrentBoostEnv = [System.Environment]::GetEnvironmentVariable("BOOST_ROOT", "Machine")

if ($CurrentBoostEnv -ne $TargetBoostPath) {
    Write-Host "Setting global BOOST_ROOT to $TargetBoostPath" -ForegroundColor Cyan
    [System.Environment]::SetEnvironmentVariable("BOOST_ROOT", $TargetBoostPath, "Machine")
    $env:BOOST_ROOT = $TargetBoostPath
}

$CurrentBoostInc = [System.Environment]::GetEnvironmentVariable("BOOST_INCLUDEDIR", "Machine")
if ($CurrentBoostInc -ne $TargetBoostPath) {
    [System.Environment]::SetEnvironmentVariable("BOOST_INCLUDEDIR", $TargetBoostPath, "Machine")
    $env:BOOST_INCLUDEDIR = $TargetBoostPath
}

$LibPath = Join-Path $TargetBoostPath "stage\lib"
$CurrentBoostLib = [System.Environment]::GetEnvironmentVariable("BOOST_LIBRARYDIR", "Machine")
if ($CurrentBoostLib -ne $LibPath) {
    [System.Environment]::SetEnvironmentVariable("BOOST_LIBRARYDIR", $LibPath, "Machine")
    $env:BOOST_LIBRARYDIR = $LibPath
}

Write-Host "Boost environment variables configured." -ForegroundColor Green

# =========================================== VSCode step


$VsCodeCommand = Get-Command "code" -ErrorAction SilentlyContinue

if ($VSCodeCommand) {
	Write-Host "VS Code is currently installed." -ForegroundColor Green
} else {
	Write-Host "VS Code is missing." -ForegroundColor Yellow
	$UserInput = Read-Host "Do you want install VS Code? (Y/N)"

	if ($UserInput -eq 'Y') {
		Write-Host "Starting VSCode installation process..." -ForegroundColor Cyan
		winget install --id Microsoft.VisualStudioCode -e --silent --source winget --accept-package-agreements --accept-source-agreements
		Write-Host "Installation finished!" -ForegroundColor Green
	} else {
		Write-Host "Installation skipped by user." -ForegroundColor Gray
	}
}

# =========================================== VSCode Extensions step

$CodePath = "code"

if (-not (Get-Command "code" -ErrorAction SilentlyContinue)) {
    if (Test-Path "$env:LOCALAPPDATA\Programs\Microsoft VS Code\bin\code.cmd") {
        $CodePath = "$env:LOCALAPPDATA\Programs\Microsoft VS Code\bin\code.cmd"
    } elseif (Test-Path "$env:ProgramFiles\Microsoft VS Code\bin\code.cmd") {
        $CodePath = "$env:ProgramFiles\Microsoft VS Code\bin\code.cmd"
    } else {
        Write-Host "Error: Could not find VS Code executable path." -ForegroundColor Red
        exit
    }
}

Write-Host "Checking VSCode extensions..." -ForegroundColor Cyan

$ExtensionsListPath = Join-Path $PSScriptRoot "env\vscode_extensions.txt"

if (Test-Path $ExtensionsListPath) {
    $InstalledExtensions = & $CodePath --list-extensions
    $ExtensionsToInstall = Get-Content $ExtensionsListPath

    foreach ($ExtensionId in $ExtensionsToInstall) {
        $ExtensionId = $ExtensionId.Trim()

        if ([string]::IsNullOrWhiteSpace($ExtensionId) -or $ExtensionId.StartsWith("#")) {
            continue
        }

        if ($InstalledExtensions -like "*$ExtensionId*") {
            Write-Host "Extension '$ExtensionId' is already installed." -ForegroundColor Green
        } else {
            Write-Host "Extension '$ExtensionId' is missing." -ForegroundColor Yellow
            $UserInput = Read-Host "Do you want install '$ExtensionId'? (Y/N)"

            if ($UserInput -eq 'Y') {
                Write-Host "Installing extension '$ExtensionId'..." -ForegroundColor Cyan
                & $CodePath --install-extension $ExtensionId --force
                Write-Host "Extension installation finished!" -ForegroundColor Green
            } else {
                Write-Host "Extension installation skipped by user." -ForegroundColor Gray
            }
        }
    }
} else {
    Write-Host "Warning: Extensions list file not found at $ExtensionsListPath" -ForegroundColor Red
}

# =========================================== Finish step

Write-Host "`n==============================================" -ForegroundColor Cyan
Write-Host "   ENVIRONMENT SETUP COMPLETED SUCCESSFULLY" -ForegroundColor Green
Write-Host "==============================================`n" -ForegroundColor Cyan

Write-Host "All requested tools and extensions have been processed." -ForegroundColor White
Write-Host "You might need to restart any open terminals or VS Code for all changes to take effect." -ForegroundColor Gray

Write-Host "`nPress any key to exit..." -ForegroundColor Yellow
$null = [System.Console]::ReadKey($true)
exit