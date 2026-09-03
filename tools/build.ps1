[CmdletBinding()]
param(
    [ValidateSet("Debug", "Release")]
    [string]$Configuration = "Debug",

    [switch]$ConfigureOnly
)

$ErrorActionPreference = "Stop"

$picoToolsRoot = Join-Path $env:USERPROFILE ".pico-sdk"

function Find-Tool {
    param(
        [Parameter(Mandatory)]
        [string]$Root,

        [Parameter(Mandatory)]
        [string]$Name
    )

    if (-not (Test-Path -LiteralPath $Root)) {
        throw "Tool directory was not found: $Root"
    }

    $tool = Get-ChildItem -LiteralPath $Root -Filter $Name -File -Recurse |
        Sort-Object LastWriteTime -Descending |
        Select-Object -First 1

    if ($null -eq $tool) {
        throw "$Name was not found under $Root. Install the Raspberry Pi Pico VS Code extension and its tools."
    }

    return $tool
}

$gcc = Find-Tool -Root (Join-Path $picoToolsRoot "toolchain") -Name "arm-none-eabi-gcc.exe"
$ninja = Find-Tool -Root (Join-Path $picoToolsRoot "ninja") -Name "ninja.exe"
$picotool = Find-Tool -Root (Join-Path $picoToolsRoot "picotool") -Name "picotool.exe"

$env:PICO_TOOLCHAIN_PATH = Split-Path $gcc.DirectoryName -Parent
$env:PICOTOOL_FETCH_FROM_GIT_PATH = Split-Path $picotool.DirectoryName -Parent
$env:PATH = "$($ninja.DirectoryName);$env:PATH"

$preset = $Configuration.ToLowerInvariant()

cmake --preset $preset
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if (-not $ConfigureOnly) {
    cmake --build --preset $preset
    exit $LASTEXITCODE
}
