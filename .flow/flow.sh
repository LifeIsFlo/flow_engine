#!/bin/bash

COMPILING="\033[33m[COMPILING]\033[0m"
OK="\033[32m[SUCCES]\033[0m"
FAILED="\033[31m[FAILED]\033[0m"

projectName=$(jq -r '.name' flow.cppflowproj)

ROOT="$(pwd -W)"

genVsCodeRuns() {
    mkdir -p "$ROOT/.vscode"

    jq -n --arg root "$ROOT" --arg project "$projectName" '{
    version: "0.2.0",
    configurations: [
        {
        name: "Run Program",
        type: "cppvsdbg",
        request: "launch",
        cwd: ($root + "/bin/Debug"),
        program: ($root + "/bin/Debug/" + $project + ".exe"),
        console: "internalConsole",
        preLaunchTask: "prepareRun"
        }
    ]
    }' > "$ROOT/.vscode/launch.json"

    jq -n '{
    version: "2.0.0",
    tasks: [
        {
        label: "prepareRun",
        type: "shell",
        command: "./flow.bat",
        args: ["prepareRun", "debug"],
        problemMatcher: []
        }
    ]
    }' > "$ROOT/.vscode/tasks.json"

    jq -n '{
    configurations: [
        {
        name: "Win32",
        includePath: [
            "${workspaceFolder}/.flow/include",
            "${workspaceFolder}/.flow/include/**"
        ],
        defines: ["_DEBUG", "UNICODE", "_UNICODE"],
        compilerPath: "cl.exe",
        cStandard: "c17",
        cppStandard: "c++17",
        intelliSenseMode: "windows-msvc-x64"
        }
    ],
    version: 4
    }' > "$ROOT/.vscode/c_cpp_properties.json"
}

build() {
    echo -e "$COMPILING Creating build directory..."

    mkdir -p "$ROOT/bin" "$ROOT/obj"

    case "$MODE" in
        debug)
            mkdir -p "$ROOT/bin/Debug" "$ROOT/obj/Debug"
            ;;
        release)
            mkdir -p "$ROOT/bin/Release" "$ROOT/obj/Release"
            ;;
    esac

    echo -e "$OK Done"

    echo -e "$COMPILING Compiling source files..."

    for f in src/*.cpp; do
        echo -e "$COMPILING Compiling $f..."

        case "$MODE" in
            debug)
                g++ -c "$f" -o "$ROOT/obj/Debug/$(basename "${f%.cpp}.o")" -I "$ROOT/.flow/include"
                ;;
            release)
                g++ -c "$f" -o "$ROOT/obj/Release/$(basename "${f%.cpp}.o")" -I "$ROOT/.flow/include"
                ;;
        esac



        if [ $? -ne 0 ]; then
            echo -e "$FAILED Failed compiling $f"
            exit 1
        else
            echo -e "$OK Compiled successfully!"
        fi
    done

    echo -e "$COMPILING Making executable..."
    case "$MODE" in
        debug)
            g++ "$ROOT/obj/Debug/"*.o -o "$ROOT/bin/Debug/$projectName"
            ;;
        release)
            g++ "$ROOT/obj/Release/"*.o -o "$ROOT/bin/Release/$projectName"
            ;;
    esac
    echo -e "$OK Build complete"
}

prepareRun(){
    build
    mkdir -p logs
}

runDebug() {
    prepareRun

    powershell.exe -NoProfile -Command "powershell -File '$ROOT/.flow/run.ps1'"
    echo run complete
}

runNoDebug(){
    prepareRun

    powershell.exe -NoProfile -Command "
        Start-Process conhost -ArgumentList 'cmd.exe /c title Runtime & $ROOT\bin\Release\$($projectName).exe 2>nul';
    "
}

run(){
    case "$MODE" in
        debug)
            runDebug
            ;;
        release)
            runNoDebug
            ;;
    esac
}

clean() {
    echo "Cleaning..."

    case "$MODE" in
        debug)
            rm -rf "$ROOT/bin/Debug" "$ROOT/obj/Debug"
            ;;
        release)
            rm -rf "$ROOT/bin/Release" "$ROOT/obj/Release" 
            ;;
        logs)
            rm -rf "$ROOT/logs"
            ;;
        *)
            rm -rf "$ROOT/bin" "$ROOT/obj"
            ;;
    esac

    echo -e "$OK Clean complete"
}

displayTasks(){
    echo "Available tasks:"
    echo "  build"
    echo "  tasks"
    echo "  run"
    echo "  runDebug"
    echo "  clean"
}

MODE="$2"

case "$1" in
    build)
        build
        ;;
    run)
        run
        ;;
    clean)
        clean
        ;;
    tasks)
        displayTasks
        ;;
    prepareRun)
        prepareRun
        ;;
    genVsCodeRuns)
        genVsCodeRuns
        ;;
    *)
        displayTasks
        ;;
esac