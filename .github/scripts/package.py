import sys
from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED

platform = sys.argv[1]
build_root = Path(sys.argv[2])
output = Path("dist")

artefact_dirs = list(build_root.rglob("*_artefacts"))
if not artefact_dirs:
    raise SystemExit(f"No *_artefacts directory found under: {build_root}")

source = artefact_dirs[0] / "Release" if (artefact_dirs[0] / "Release").is_dir() else artefact_dirs[0]

excluded = {
    ".lib", ".pdb", ".exp", ".ilk", ".obj",
    ".o", ".a", ".d", ".map", ".idb",
    ".pch", ".tlog"
}

output.mkdir(parents=True, exist_ok=True)
archive = output / f"Undxd-{platform}.zip"

files = [
    p for p in source.rglob("*")
    if p.is_file()
    and p.suffix.lower() not in excluded
    and p.name not in {".DS_Store", "Thumbs.db"}
]

if not files:
    raise SystemExit(f"No files found to package in: {source}")

with ZipFile(archive, "w", ZIP_DEFLATED, compresslevel=6) as z:
    for file in files:
        z.write(file, file.relative_to(source))

print(f"Created {archive} from {source}")
print(f"Included {len(files)} files")
print(f"Size: {archive.stat().st_size / 1024 / 1024:.2f} MB")