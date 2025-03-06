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
