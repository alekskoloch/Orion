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