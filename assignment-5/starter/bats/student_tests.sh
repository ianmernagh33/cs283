#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

#!/usr/bin/env bats

# File: student_tests.sh
# Create your unit tests suite in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF
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

@test "Chained commands: mkdir test_dir && ls test_dir" {
    run ./dsh <<EOF
mkdir test_dir && ls | grep test_dir
EOF
    echo "Output: $output"
    [[ "$output" == *"test_dir"* ]]
    [ "$status" -eq 0 ]
    rmdir test_dir  # Cleanup
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
