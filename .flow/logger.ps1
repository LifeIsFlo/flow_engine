$config = Get-Content flow.cppflowproj -Raw | ConvertFrom-Json
$host.UI.RawUI.WindowTitle = "Logger - $($config.name)"
$path = "logs\debug.txt"

while (-not (Test-Path $path)) {
    Start-Sleep -Milliseconds 100
}

Get-Content $path -Wait