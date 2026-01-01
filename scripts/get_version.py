#!/usr/bin/env python3
"""
Script to extract firmware version from git tags.
Used by PlatformIO to inject version into build.
"""
import subprocess
import sys


def get_version():
    """Get version from git describe or fallback to 'indev'."""
    try:
        # Try to get version from git tag
        # --tags: use all tags (not just annotated)
        # --always: show commit hash if no tags
        # --dirty: append '-dirty' if working tree has modifications
        version = (
            subprocess.check_output(
                ["git", "describe", "--tags", "--always", "--dirty"],
                stderr=subprocess.DEVNULL,
            )
            .strip()
            .decode("utf-8")
        )

        # Remove 'v' prefix if present (e.g., v1.3.2 -> 1.3.2)
        if version.startswith("v"):
            version = version[1:]

        return version
    except (subprocess.CalledProcessError, FileNotFoundError):
        # Fallback if git is not available or not a git repository
        return "indev"


if __name__ == "__main__":
    version = get_version()
    # Output as a C preprocessor define for PlatformIO
    print(f'-DFIRMWARE_VERSION=\\"{version}\\"')
