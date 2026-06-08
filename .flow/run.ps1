$runtime = Start-Process powershell `
    -ArgumentList '-File', '.flow/runtime.ps1' `
    -PassThru
Write-Host 'run started';

$tail = Start-Process powershell `
    -ArgumentList '-File', '.flow/logger.ps1' `
    -PassThru
Write-Host 'logger started';

Wait-Process -Id $runtime.Id;
Stop-Process -Id $tail.Id;

$timestamp = Get-Date -Format "yyyy-MM-dd_HH-mm-ss"
$outputFile = "logs\$timestamp.txt"

Copy-Item logs\debug.txt $outputFile
Copy-Item logs\debug.txt "logs\latest.txt"