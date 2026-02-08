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