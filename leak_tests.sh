#!/bin/bash

# Update leak_tests.sh with direct process testing
echo "=== Direct Process Tests ==="

# Test 1: Simple builtin
valgrind --leak-check=full --trace-children=yes ./minishell << EOF
echo test
exit
EOF

# Test 2: Pipe with cleanup
valgrind --leak-check=full --trace-children=yes ./minishell << EOF
ls | wc
exit
EOF

# Test 3: Heredoc cleanup
valgrind --leak-check=full --trace-children=yes ./minishell << EOF
cat << END
test
END
exit
EOF

# Test 4: Environment cleanup
valgrind --leak-check=full --trace-children=yes ./minishell << EOF
export TEST=value
env
unset TEST
exit
EOF