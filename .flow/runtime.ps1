$config = Get-Content flow.cppflowproj -Raw | ConvertFrom-Json
$host.UI.RawUI.WindowTitle = "Runtime - $($config.name)"
& bin/Debug/$($config.name).exe 2> $null 3> $null