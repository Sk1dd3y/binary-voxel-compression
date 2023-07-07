#!/bin/bash
echo "Clean up old files..."

rm test3.data
rm decomp3.data

# Execute the voxel binary with -c option
./voxel -c example3.data test3.data

# Execute the voxel binary with -d option
./voxel -d test3.data decomp3.data

# Get the file sizes using `du -h`
file_sizes=$(du -h example3.data test3.data decomp3.data)

# Calculate the MD5 checksums using `md5sum`
md5_checksums=$(md5sum example3.data decomp3.data)

# Print the file sizes and MD5 checksums
echo "File Sizes:"
echo "$file_sizes"
echo
echo "MD5 Checksums:"
echo "$md5_checksums"
