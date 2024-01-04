mysync - File Synchronization Utility
Overview
mysync is a command-line utility developed in C11 for synchronizing the contents of two or more directories. 


Features
Synchronize files between specified directories.
Support for  and two or more directory names.
Options include:
-a: Synchronize hidden and configuration files.
-i pattern: Ignore filenames matching the indicated pattern.
-n: Identify files to be copied without actually synchronizing.
-o pattern: Synchronize only filenames matching the indicated pattern.
-p: Preserve the modification time and file permissions of the original file.
-r: Recursively process directories found within the named directories.
-v: Enable verbose output.
Use of globbing for file patterns with support for wildcards.
Recursive synchronization of directories.
