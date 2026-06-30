#!/usr/bin/env pwsh
# Build + run the hm64-decomp Docker image on Windows.
#
# Usage:
#   .\tools\docker.ps1 make fresh
#   .\tools\docker.ps1 make fresh VERSION=jp
#   .\tools\docker.ps1                       # interactive container shell

$ErrorActionPreference = 'Stop'

if (-not (Get-Command docker -ErrorAction SilentlyContinue)) {
    Write-Error "docker was not found on PATH. Install Docker Desktop and make sure 'docker' works in PowerShell."
    exit 1
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$image    = if ($env:HM64_DOCKER_IMAGE)    { $env:HM64_DOCKER_IMAGE }    else { 'hm64-decomp:bookworm' }
$platform = if ($env:HM64_DOCKER_PLATFORM) { $env:HM64_DOCKER_PLATFORM } else { 'linux/amd64' }

docker build --platform $platform -t $image -f (Join-Path $repoRoot 'Dockerfile') $repoRoot
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

$runArgs = @('run', '--rm', '--platform', $platform, '-v', "${repoRoot}:/repo", '-w', '/repo')

# Allocate a TTY only when attached to an interactive console so piped/CI invocations still work.
$interactive = $false
try {
    $interactive = (-not [Console]::IsInputRedirected) -and (-not [Console]::IsOutputRedirected)
} catch {
    $interactive = $false
}
if ($interactive) { $runArgs += '-it' }

$runArgs += $image
$runArgs += $args

docker @runArgs
exit $LASTEXITCODE
