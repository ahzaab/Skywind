npm install

$path = split-path $SCRIPT:MyInvocation.MyCommand.Path -parent

Push-Location $path

if (!$(Test-Path "$path\.\bin"))
{
    New-Item "$path\.\bin" -ItemType Directory
}

$outputPath = Resolve-Path "$path\.\bin"

$flashPath = 'D:\Program Files\Adobe\Adobe Flash CS6\Flash.exe'

$temp = " --interactive-compiler `"$flashPath`" --input-directory `"$path`" --output-directory `"$outputPath`" --include-pattern `"*.fla`" --debug true"

Invoke-Expression ".\node_modules\.bin\flc $temp"

Pop-Location
