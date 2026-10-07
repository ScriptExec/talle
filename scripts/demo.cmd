@echo off
setlocal enabledelayedexpansion

pushd build\cmake\install\x86_64-windows-debug\bin
call vhs %~dp0..\resource\example-colors.tape
move .\example-colors.gif %~dp0..\resource\example-colors.gif
call vhs %~dp0..\resource\example-styles.tape
move .\example-styles.gif %~dp0..\resource\example-styles.gif
popd
endlocal
