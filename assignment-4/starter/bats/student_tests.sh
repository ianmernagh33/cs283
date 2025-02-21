#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF
ls
EOF

    [ "$status" -eq 0 ]
}

@test "Run ls with -l flag" {
    run ./dsh <<EOF
ls -l
EOF
    [ "$status" -eq 0 ]
}

@test "Check echo command" {
    run ./dsh <<EOF
    echo "Hello, dsh!"
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, dsh!"* ]]
}

@test "Echo empty string" {
    run ./dsh <<EOF
echo ""
EOF
    [[ "$output" == *""* ]]
    [ "$status" -eq 0 ]
}

@test "Echo quoted with spaces" {
    run ./dsh <<EOF
echo "hello   world"
EOF
    [[ "$output" == *"hello   world"* ]]
    [ "$status" -eq 0 ]
}

@test "Multiple spaces between arguments" {
    run ./dsh <<EOF
echo    hello        world
EOF
    [[ "$output" == *"hello world"* ]]
    [ "$status" -eq 0 ]
}

@test "Check built-in exit command" {
    run ./dsh <<EOF
    exit
EOF

    [ "$status" -eq 0 ]
}

@test "Check cd command changes directory" {
    run ./dsh <<EOF
    cd /
    pwd
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == */* ]]
}

@test "Check multiple commands execution" {
    run ./dsh <<EOF
    echo "first"
    echo "second"
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"first"* ]]
    [[ "$output" == *"second"* ]]
}

@test "Check handling of empty input" {
    run ./dsh <<EOF

EOF

    [ "$status" -eq 0 ]
}

@test "Check uname command" {
    run ./dsh <<EOF
uname
EOF

    [ "$status" -eq 0 ]
    [ -n "$output" ]
}

@test "Check uname -a command" {
    run ./dsh <<EOF
uname -a
EOF

    [ "$status" -eq 0 ]
    [ -n "$output" ]
}
