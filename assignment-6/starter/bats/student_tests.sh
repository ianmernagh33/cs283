#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Pipe command: ls | grep .c" {
    run ./dsh <<EOF
ls | grep .c
EOF
    [[ "$output" == *".c"* ]]
    [ "$status" -eq 0 ]
}

@test "Multiple pipes: ls | grep .c | sort" {
    run ./dsh <<EOF
ls | grep .c | sort
EOF
    [[ "$output" == *".c"* ]]
    [ "$status" -eq 0 ]
}

@test "Pipe command with word count: ls | wc -l" {
    expectedOutput=$(ls | wc -l)
    run ./dsh <<EOF
ls | wc -l
EOF
    cleanedOutput=$(echo "$output" | grep -o '^[0-9]\+')
    [ "$cleanedOutput" -eq "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Pipe command with cat: echo 'hello' | cat" {
    run ./dsh <<EOF
echo "hello" | cat
EOF
    expectedOutput="hello"
    cleanedOutput=$(echo "$output" | sed -n '1p')
    [ "$cleanedOutput" = "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Test output redirection" {
    run ./dsh <<EOF
echo "first line" > out.txt
cat out.txt
EOF
    expectedOutput="first line"
    cleanedOutput=$(echo "$output" | sed -n '1p')
    [ "$cleanedOutput" = "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Exit command" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Background execution: sleep 2 &" {
    run ./dsh <<EOF
sleep 2 &
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
}

@test "File input redirection: cat < /etc/passwd" {
    expectedOutput=$(head -n 1 /etc/passwd)
    run ./dsh <<EOF
cat < /etc/passwd
EOF
    cleanedOutput=$(echo "$output" | head -n1)
    [ "$cleanedOutput" = "$expectedOutput" ]
    [ "$status" -eq 0 ]
}

@test "Appending to a file with >>" {
    run ./dsh <<EOF
echo "first line" > append_test.txt
echo "second line" >> append_test.txt
EOF
    run cat append_test.txt
    expectedOutput="first line"$'\n'"second line"
    echo "Output: $output"
    [ "$output" = "$expectedOutput" ]
    [ "$status" -eq 0 ]
    rm append_test.txt  # Cleanup
}

@test "SIGINT (Ctrl+C) handling: sleep 5" {
    run timeout 2 ./dsh <<EOF
sleep 5
EOF
    [ "$status" -ne 0 ]  # Expecting it to be interrupted
}

@test "Handling empty command" {
    run ./dsh <<EOF

EOF
    [ "$status" -eq 0 ]
}

@test "Multiple background processes: sleep 1 & sleep 1 &" {
    run ./dsh <<EOF
sleep 1 & sleep 1 &
EOF
    [ "$status" -eq 0 ]
}

@test "Check exit status of last command in pipeline" {
    run ./dsh <<EOF
false | true
EOF
    [ "$status" -eq 0 ] # true should set status to 0
}

@test "Check exit status of last command in chain" {
    run ./dsh <<EOF
false && true
EOF
    [ "$status" -eq 0 ] # true should set status to 0
}

@test "cd with relative path" {
    mkdir cd_test_dir
    run ./dsh <<EOF
cd cd_test_dir
pwd
EOF
    rmdir cd_test_dir
    [[ "$output" == *"/cd_test_dir"* ]]
    [ "$status" -eq 0 ]
}

@test "cd with absolute path" {
    run ./dsh <<EOF
cd /tmp
pwd
EOF
    [[ "$output" == *"/tmp"* ]]
    [ "$status" -eq 0 ]
}

@test "cd with no arguments" {
    run ./dsh <<EOF
cd
EOF
    [ "$status" -eq 0 ] # should go to home dir
}

@test "Foreground process exit status check true" {
    run ./dsh <<EOF
true
echo \$?
EOF
    [[ "$output" == *"0"* ]] # true should have exit status 0
}
