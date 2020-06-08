$sourcePath = "$($Env:SkywindDir)\Data"
$distPath = "$($Env:ModDevPath)\MODS\SkyrimSE\Skywind\repo\SKSE64\skse64\Skywind\dist\Data"
$zipFileName = "SkywindPlugin.7z"
$requiredPaths = @("Interface","Scripts","Source\Scripts", "SKSE\Plugins");

$requiredPaths | ForEach-Object {
    if (!$(Test-Path "$distPath\$_"))
    {
        New-Item -ItemType Directory "$distPath\$_"
    }
}

Copy-Item "$sourcePath\Scripts\AHZ_SKWD*.*" -Destination "$distPath\Scripts"
Copy-Item "$sourcePath\Scripts\Skywind.pex" -Destination "$distPath\Scripts"
Copy-Item "$sourcePath\Source\Scripts\AHZ_SKWD*.*" -Destination "$distPath\Source\Scripts"
Copy-Item "$sourcePath\Source\Scripts\Skywind.psc" -Destination "$distPath\Source\Scripts"
Copy-Item "$sourcePath\AHZ_SKWD*.*" -Destination "$distPath"
Copy-Item "$sourcePath\Interface\StatsMenuEx.swf" -Destination "$distPath\Interface"
Copy-Item "$sourcePath\SKSE\Plugins\Skywind.dll" -Destination "$distPath\SKSE\Plugins"


Start-Process "C:\Program Files\7-Zip\7z" -ArgumentList "a `"$distPath\..\$zipFileName`" `"$distPath`" -mx5 -t7z" -wait -NoNewWindow -PassThru