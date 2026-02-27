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