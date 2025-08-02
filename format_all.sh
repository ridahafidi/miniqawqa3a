#!/bin/bash

# Script to format all C and header files with c_formatter_42
# This will make them comply with norminette standards

FORMATTER="$HOME/.local/bin/c_formatter_42"

if [ ! -f "$FORMATTER" ]; then
    echo "Error: c_formatter_42 not found at $FORMATTER"
    echo "Please install it first: pip3 install --user c_formatter_42"
    exit 1
fi

echo "🔧 Formatting all C and header files..."
echo "========================================="

# Counter for formatted files
formatted_count=0
error_count=0

# Format all .c and .h files in the current directory
for file in *.c *.h; do
    if [ -f "$file" ]; then
        echo "📄 Formatting: $file"
        
        # Create a backup
        cp "$file" "$file.bak"
        
        # Format the file
        if "$FORMATTER" < "$file.bak" > "$file" 2>/dev/null; then
            echo "   ✅ Successfully formatted"
            rm "$file.bak"  # Remove backup if successful
            ((formatted_count++))
        else
            echo "   ❌ Error formatting - restoring original"
            mv "$file.bak" "$file"  # Restore original if error
            ((error_count++))
        fi
    fi
done

echo "========================================="
echo "📊 Summary:"
echo "   ✅ Successfully formatted: $formatted_count files"
echo "   ❌ Errors: $error_count files"
echo ""

if [ $formatted_count -gt 0 ]; then
    echo "🎉 Formatting complete! Running norminette to check results..."
    echo ""
    echo "Files that now pass norminette:"
    norminette *.c *.h 2>/dev/null | grep ": OK!" | wc -l | xargs echo "   ✅"
    echo ""
    echo "Files that still have errors:"
    norminette *.c *.h 2>/dev/null | grep ": Error!" | wc -l | xargs echo "   ❌"
fi
