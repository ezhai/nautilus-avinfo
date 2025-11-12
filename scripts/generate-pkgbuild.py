#!/bin/python3

import sys
import os
from pathlib import Path
from dataclasses import dataclass, asdict

from jinja2 import Environment, FileSystemLoader, select_autoescape


@dataclass
class Data:
    local: bool
    version: str
    release: str
    filename: str


if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: generate-pkgbuild.py <version> <release> <pkgdir>")
        sys.exit(2)

    version = sys.argv[1]
    release = sys.argv[2]
    pkgdir = sys.argv[3]
    params = [
        Data(local=False, version=version, release=release, filename="PKGBUILD"),
        Data(local=True, version=version, release=release, filename="PKGBUILD.local"),
    ]

    templatedir = Path(os.path.realpath(__file__)).parent.joinpath(pkgdir)
    env = Environment(
        loader=FileSystemLoader(templatedir),
        autoescape=select_autoescape(),
        keep_trailing_newline=True,
    )
    template = env.get_template("PKGBUILD.jinja")

    for param in params:
        content = template.render(asdict(param))
        specfile = templatedir.joinpath(param.filename)
        with open(specfile, "w") as f:
            f.write(content)
