#!/usr/bin/env python3
"""
Architecture Migration Script for PIC32MM Emergency Dialer
===========================================================
Reorganizes flat src/ directory into a 5-layer embedded architecture:
  L0 MCAL:    src/config/default/  (UNTOUCHED - Harmony generated)
  L1 HAL:     src/hal/             (BSP, SPI guard, IO monitor, SD SPI low-level)
  L2 Drivers: src/drivers/         (Flash, SD raw, Telit, ESP32 comm, W5500)
  L3 Services:src/services/        (GSM, SMS, Alarm, Storage, Store, Phonebook, SD service, ESP32 proto)
  L4 App:     src/app/             (main.c, app.c, app.h, handlers.c, protothreads)
  Common:     src/common/          (schema.h, proto.h, pt.h, lc.h, lc-switch.h)
  Test:       src/test/            (sd_test.c/h, sd_card.c, layers_docments.c)
"""

import os
import shutil
import re
from pathlib import Path

BASE = r"C:\Users\ahmed.thabit\Desktop\Minesh\ArchitectureFolder"
SRC = os.path.join(BASE, "src")
ARCHX = os.path.join(BASE, "Architecture.X")
DRY_RUN = False

NEW_DIRS = [
    "src/common",
    "src/hal/bsp",
    "src/hal/spi_guard",
    "src/hal/io_monitor",
    "src/hal/sd_spi",
    "src/drivers/flash",
    "src/drivers/sd",
    "src/drivers/telit",
    "src/drivers/esp32",
    "src/drivers/ethernet",
    "src/services/gsm",
    "src/services/sms",
    "src/services/alarm",
    "src/services/storage",
    "src/services/config",
    "src/services/phonebook",
    "src/services/sd_service",
    "src/services/esp32_proto",
    "src/app",
    "src/app/threads",
    "src/test",
]

MOVE_MAP = [
    # L1 HAL
    ("src/bsp.c",              "src/hal/bsp/bsp.c"),
    ("src/bsp.h",              "src/hal/bsp/bsp.h"),
    ("src/spi_bus_guard.c",    "src/hal/spi_guard/spi_bus_guard.c"),
    ("src/spi_bus_guard.h",    "src/hal/spi_guard/spi_bus_guard.h"),
    ("src/io_monitor.c",       "src/hal/io_monitor/io_monitor.c"),
    ("src/io_monitor.h",       "src/hal/io_monitor/io_monitor.h"),
    ("src/sd_spi_lowlevel.c",  "src/hal/sd_spi/sd_spi_lowlevel.c"),
    ("src/sd_spi_lowlevel.h",  "src/hal/sd_spi/sd_spi_lowlevel.h"),
    ("src/sd_fatfs_guard.h",   "src/hal/sd_spi/sd_fatfs_guard.h"),
    
    # L2 Drivers
    ("src/flash_w25q32.c",     "src/drivers/flash/flash_w25q32.c"),
    ("src/flash_w25q32.h",     "src/drivers/flash/flash_w25q32.h"),
    ("src/sd_raw.c",           "src/drivers/sd/sd_raw.c"),
    ("src/sd_raw.h",           "src/drivers/sd/sd_raw.h"),
    ("src/telit.c",            "src/drivers/telit/telit.c"),
    ("src/telit.h",            "src/drivers/telit/telit.h"),
    ("src/esp32.c",            "src/drivers/esp32/esp32.c"),
    ("src/esp32.h",            "src/drivers/esp32/esp32.h"),
    ("src/esp32_comm.c",       "src/drivers/esp32/esp32_comm.c"),
    ("src/esp32_comm.h",       "src/drivers/esp32/esp32_comm.h"),
    ("src/w5500if.c",          "src/drivers/ethernet/w5500if.c"),
    ("src/w5500if.h",          "src/drivers/ethernet/w5500if.h"),
    
    # L3 Services
    ("src/gsm_le910.c",        "src/services/gsm/gsm_le910.c"),
    ("src/gsm_le910.h",        "src/services/gsm/gsm_le910.h"),
    ("src/sms_service.c",      "src/services/sms/sms_service.c"),
    ("src/alarm_mgr.c",        "src/services/alarm/alarm_mgr.c"),
    ("src/storage.c",          "src/services/storage/storage.c"),
    ("src/storage.h",          "src/services/storage/storage.h"),
    ("src/store.c",            "src/services/config/store.c"),
    ("src/store.h",            "src/services/config/store.h"),
    ("src/phonebook_flash.c",  "src/services/phonebook/phonebook_flash.c"),
    ("src/phonebook_flash.h",  "src/services/phonebook/phonebook_flash.h"),
    ("src/sd_service.c",       "src/services/sd_service/sd_service.c"),
    ("src/sd_service.h",       "src/services/sd_service/sd_service.h"),
    ("src/esp32_proto.c",      "src/services/esp32_proto/esp32_proto.c"),
    ("src/esp32_proto.h",      "src/services/esp32_proto/esp32_proto.h"),
    
    # L4 App
    ("src/main.c",             "src/app/main.c"),
    ("src/app.c",              "src/app/app.c"),
    ("src/app.h",              "src/app/app.h"),
    ("src/handlers.c",         "src/app/handlers.c"),
    ("Architecture.X/protothreads.c",  "src/app/threads/protothreads.c"),
    ("Architecture.X/protothreads.h",  "src/app/threads/protothreads.h"),
    
    # Common
    ("src/schema.h",           "src/common/schema.h"),
    ("src/proto.h",            "src/common/proto.h"),
    ("src/pt.h",               "src/common/pt.h"),
    ("src/lc.h",               "src/common/lc.h"),
    ("src/lc-switch.h",        "src/common/lc-switch.h"),
    ("src/integer.h",          "src/common/integer.h"),
    ("src/pt_yield_flag.c",    "src/common/pt_yield_flag.c"),
    
    # Test
    ("src/sd_test.c",          "src/test/sd_test.c"),
    ("src/sd_test.h",          "src/test/sd_test.h"),
    ("src/sd_card.c",          "src/test/sd_card.c"),
    ("src/layers_docments.c",  "src/test/layers_docments.c"),
]

# Header filename -> new path relative to src/
HEADER_LOCATIONS = {
    "schema.h":          "common/schema.h",
    "proto.h":           "common/proto.h",
    "pt.h":              "common/pt.h",
    "lc.h":              "common/lc.h",
    "lc-switch.h":       "common/lc-switch.h",
    "integer.h":         "common/integer.h",
    "bsp.h":             "hal/bsp/bsp.h",
    "spi_bus_guard.h":   "hal/spi_guard/spi_bus_guard.h",
    "io_monitor.h":      "hal/io_monitor/io_monitor.h",
    "sd_spi_lowlevel.h": "hal/sd_spi/sd_spi_lowlevel.h",
    "sd_fatfs_guard.h":  "hal/sd_spi/sd_fatfs_guard.h",
    "flash_w25q32.h":    "drivers/flash/flash_w25q32.h",
    "sd_raw.h":          "drivers/sd/sd_raw.h",
    "telit.h":           "drivers/telit/telit.h",
    "esp32.h":           "drivers/esp32/esp32.h",
    "esp32_comm.h":      "drivers/esp32/esp32_comm.h",
    "w5500if.h":         "drivers/ethernet/w5500if.h",
    "gsm_le910.h":       "services/gsm/gsm_le910.h",
    "storage.h":         "services/storage/storage.h",
    "store.h":           "services/config/store.h",
    "phonebook_flash.h": "services/phonebook/phonebook_flash.h",
    "sd_service.h":      "services/sd_service/sd_service.h",
    "esp32_proto.h":     "services/esp32_proto/esp32_proto.h",
    "app.h":             "app/app.h",
    "protothreads.h":    "app/threads/protothreads.h",
    "sd_test.h":         "test/sd_test.h",
}

log_lines = []
def log(msg):
    print(msg)
    log_lines.append(msg)

def ensure_dir(path):
    if not os.path.exists(path):
        if not DRY_RUN:
            os.makedirs(path, exist_ok=True)
        log(f"  [MKDIR] {path}")

def move_file(src_path, dst_path):
    if not os.path.exists(src_path):
        log(f"  [SKIP]  {src_path} (not found)")
        return False
    ensure_dir(os.path.dirname(dst_path))
    if not DRY_RUN:
        shutil.copy2(src_path, dst_path)
    log(f"  [COPY]  {os.path.basename(src_path)} -> {os.path.relpath(dst_path, BASE)}")
    return True

def compute_relative_include(from_file_dir_rel, target_header_rel):
    from_parts = Path(from_file_dir_rel).parts
    target_parts = Path(target_header_rel).parts
    common = 0
    for a, b in zip(from_parts, target_parts):
        if a == b:
            common += 1
        else:
            break
    ups = len(from_parts) - common
    downs = target_parts[common:]
    result = "/".join([".."] * ups + list(downs))
    return result

def fix_includes_in_file(filepath):
    if not os.path.exists(filepath):
        return 0
    with open(filepath, 'r', encoding='utf-8', errors='replace') as f:
        content = f.read()
    original = content
    abs_src = os.path.join(BASE, "src")
    file_dir = os.path.dirname(filepath)
    try:
        file_dir_rel = os.path.relpath(file_dir, abs_src).replace("\\", "/")
    except ValueError:
        return 0
    
    changes = 0
    include_pattern = re.compile(r'(#include\s+")([^"]+)(")')
    
    def replace_include(match):
        nonlocal changes
        prefix = match.group(1)
        inc_path = match.group(2)
        suffix = match.group(3)
        inc_filename = os.path.basename(inc_path).strip()
        
        skip_prefixes = ("config/", "peripheral/", "system/", "driver/", "osal/")
        skip_exact = ("definitions.h", "device.h", "user.h", "ff.h", "diskio.h", "ffconf.h", "configuration.h")
        
        if any(inc_path.startswith(p) for p in skip_prefixes):
            return match.group(0)
        if inc_filename in skip_exact:
            return match.group(0)
        
        if inc_filename in HEADER_LOCATIONS:
            new_header_rel = HEADER_LOCATIONS[inc_filename]
            new_inc = compute_relative_include(file_dir_rel, new_header_rel)
            if new_inc != inc_path:
                changes += 1
                return f'{prefix}{new_inc}{suffix}'
        
        return match.group(0)
    
    content = include_pattern.sub(replace_include, content)
    
    if content != original and not DRY_RUN:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
    
    return changes

def collect_all_c_h_files(base_dir):
    result = []
    for root, dirs, files in os.walk(base_dir):
        rel = os.path.relpath(root, base_dir).replace("\\", "/")
        if rel.startswith("config/default") or rel.startswith("config\\default"):
            continue
        for f in files:
            if f.endswith('.c') or f.endswith('.h'):
                result.append(os.path.join(root, f))
    return result

def find_pt_files():
    candidates = [
        os.path.join(BASE, "pt.h"),
        os.path.join(BASE, "lc.h"),
        os.path.join(BASE, "lc-switch.h"),
    ]
    return [c for c in candidates if os.path.exists(c)]

def main():
    log("=" * 70)
    log("PIC32MM Architecture Migration")
    log("=" * 70)
    log(f"Base: {BASE}")
    log(f"Dry run: {DRY_RUN}")
    log("")
    
    # Phase 1: Create directories
    log("PHASE 1: Creating directory structure")
    log("-" * 40)
    for d in NEW_DIRS:
        ensure_dir(os.path.join(BASE, d))
    log("")
    
    # Phase 2: Copy pt files from project root
    log("PHASE 2: Locating protothreads headers")
    log("-" * 40)
    pt_files = find_pt_files()
    for pf in pt_files:
        fname = os.path.basename(pf)
        src_copy = os.path.join(SRC, fname)
        if os.path.exists(src_copy):
            # Already in src/, will be moved by MOVE_MAP
            log(f"  [OK]   {fname} exists in src/, will be moved")
        else:
            dest = os.path.join(SRC, "common", fname)
            move_file(pf, dest)
    log("")
    
    # Phase 3: Move files
    log("PHASE 3: Moving files to new locations")
    log("-" * 40)
    moved = 0
    skipped = 0
    for src_rel, dst_rel in MOVE_MAP:
        src_abs = os.path.join(BASE, src_rel)
        dst_abs = os.path.join(BASE, dst_rel)
        if move_file(src_abs, dst_abs):
            moved += 1
        else:
            skipped += 1
    log(f"\n  Moved: {moved}, Skipped: {skipped}")
    log("")
    
    # Phase 4: Fix includes
    log("PHASE 4: Fixing #include paths")
    log("-" * 40)
    all_files = collect_all_c_h_files(SRC)
    total_changes = 0
    for fp in all_files:
        # Only fix files in new directories (not old flat copies still pending deletion)
        rel = os.path.relpath(fp, SRC).replace("\\", "/")
        if "/" not in rel and rel != "." :
            # This is a flat file still in src/ root - skip, it will be deleted
            continue
        n = fix_includes_in_file(fp)
        if n > 0:
            log(f"  [FIX]  {os.path.relpath(fp, BASE)}: {n} includes updated")
            total_changes += n
    log(f"\n  Total include changes: {total_changes}")
    log("")
    
    # Phase 5: Clean up old flat files
    log("PHASE 5: Cleaning up old flat files")
    log("-" * 40)
    removed = 0
    for src_rel, dst_rel in MOVE_MAP:
        src_abs = os.path.join(BASE, src_rel)
        dst_abs = os.path.join(BASE, dst_rel)
        if src_rel.startswith("src/") and os.path.exists(src_abs) and os.path.exists(dst_abs):
            if os.path.normpath(src_abs) != os.path.normpath(dst_abs):
                if not DRY_RUN:
                    os.remove(src_abs)
                log(f"  [DEL]  {src_rel}")
                removed += 1
    log(f"\n  Removed: {removed} old flat files")
    log("")
    
    # Phase 6: Summary
    log("PHASE 6: Final directory structure")
    log("-" * 40)
    for root, dirs, files in os.walk(SRC):
        rel = os.path.relpath(root, SRC).replace("\\", "/")
        if rel.startswith("config"):
            if rel == "config":
                log(f"  config/ (HARMONY - UNTOUCHED)")
            continue
        level = rel.count("/") + (0 if rel == "." else 1)
        indent = "  " * level
        dirname = os.path.basename(root) if rel != "." else "src/"
        log(f"  {indent}{dirname}/")
        for f in sorted(files):
            log(f"  {indent}  {f}")
    
    log("")
    log("=" * 70)
    log("MIGRATION COMPLETE")
    log("=" * 70)
    log("")
    log("NEXT STEPS FOR MPLAB X:")
    log("1. Right-click project -> Properties -> xc32-gcc -> Preprocessing and messages")
    log("   -> Additional include directories. Add:")
    dirs_to_add = [
        "../src/common", "../src/hal/bsp", "../src/hal/spi_guard",
        "../src/hal/io_monitor", "../src/hal/sd_spi",
        "../src/drivers/flash", "../src/drivers/sd", "../src/drivers/telit",
        "../src/drivers/esp32", "../src/drivers/ethernet",
        "../src/services/gsm", "../src/services/sms", "../src/services/alarm",
        "../src/services/storage", "../src/services/config", "../src/services/phonebook",
        "../src/services/sd_service", "../src/services/esp32_proto",
        "../src/app", "../src/app/threads", "../src/test",
    ]
    for d in dirs_to_add:
        log(f"     {d}")
    log("")
    log("2. In MPLAB X Source Files tree, remove old flat file references")
    log("   and add the new folder structure")
    log("3. Build -> Clean and Build to verify")
    log("")
    
    report_path = os.path.join(BASE, "MIGRATION_REPORT.txt")
    if not DRY_RUN:
        with open(report_path, 'w', encoding='utf-8') as f:
            f.write("\n".join(log_lines))
    log(f"Report saved: {report_path}")

if __name__ == "__main__":
    main()
