#!/usr/bin/env python3
"""
Create a flash image for ESP32 QEMU testing.

This script combines bootloader, partition table, and application binary
into a single flash image suitable for QEMU emulation.
"""

import argparse
import sys
from pathlib import Path


def create_flash_image(
    output_file: Path,
    bootloader: Path,
    partition_table: Path,
    app_binary: Path,
    flash_size: int = 4 * 1024 * 1024,  # 4MB default
) -> None:
    """
    Create a flash image with proper ESP32 partition layout.

    Args:
        output_file: Output flash image path
        bootloader: Bootloader binary path (offset 0x1000)
        partition_table: Partition table binary path (offset 0x8000)
        app_binary: Application binary path (offset 0x10000)
        flash_size: Total flash size in bytes (default 4MB)
    """
    # Create empty flash image
    flash_data = bytearray(b'\xff' * flash_size)

    # Standard ESP32 partition offsets
    offsets = {
        'bootloader': 0x1000,
        'partition_table': 0x8000,
        'app': 0x10000,
    }

    # Write bootloader
    if bootloader.exists():
        with open(bootloader, 'rb') as f:
            bootloader_data = f.read()
            flash_data[offsets['bootloader']:offsets['bootloader'] + len(bootloader_data)] = bootloader_data
        print(f"✓ Bootloader: {len(bootloader_data)} bytes at 0x{offsets['bootloader']:x}")
    else:
        print(f"⚠ Warning: Bootloader not found at {bootloader}", file=sys.stderr)

    # Write partition table
    if partition_table.exists():
        with open(partition_table, 'rb') as f:
            pt_data = f.read()
            flash_data[offsets['partition_table']:offsets['partition_table'] + len(pt_data)] = pt_data
        print(f"✓ Partition table: {len(pt_data)} bytes at 0x{offsets['partition_table']:x}")
    else:
        print(f"⚠ Warning: Partition table not found at {partition_table}", file=sys.stderr)

    # Write application
    if not app_binary.exists():
        print(f"✗ Error: Application binary not found at {app_binary}", file=sys.stderr)
        sys.exit(1)

    with open(app_binary, 'rb') as f:
        app_data = f.read()
        flash_data[offsets['app']:offsets['app'] + len(app_data)] = app_data
    print(f"✓ Application: {len(app_data)} bytes at 0x{offsets['app']:x}")

    # Write flash image
    output_file.parent.mkdir(parents=True, exist_ok=True)
    with open(output_file, 'wb') as f:
        f.write(flash_data)

    print(f"\n✓ Flash image created: {output_file}")
    print(f"  Total size: {len(flash_data)} bytes ({len(flash_data) / 1024 / 1024:.1f} MB)")


def main():
    parser = argparse.ArgumentParser(
        description='Create ESP32 flash image for QEMU testing'
    )
    parser.add_argument(
        '--output', '-o',
        type=Path,
        required=True,
        help='Output flash image file'
    )
    parser.add_argument(
        '--bootloader', '-b',
        type=Path,
        required=True,
        help='Bootloader binary'
    )
    parser.add_argument(
        '--partition-table', '-p',
        type=Path,
        required=True,
        help='Partition table binary'
    )
    parser.add_argument(
        '--app', '-a',
        type=Path,
        required=True,
        help='Application binary'
    )
    parser.add_argument(
        '--flash-size',
        type=int,
        default=4 * 1024 * 1024,
        help='Flash size in bytes (default: 4MB)'
    )

    args = parser.parse_args()

    create_flash_image(
        output_file=args.output,
        bootloader=args.bootloader,
        partition_table=args.partition_table,
        app_binary=args.app,
        flash_size=args.flash_size,
    )


if __name__ == '__main__':
    main()
