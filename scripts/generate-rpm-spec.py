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
    filename: str


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: generate-rpm-spec.py <version> <specdir>")
        sys.exit(2)

    version = sys.argv[1]
    specdir = sys.argv[2]
    params = [
        Data(local=False, version=version, filename="nautilus-avinfo.spec"),
        Data(local=True, version=version, filename="nautilus-avinfo.local.spec"),
    ]

    templatedir = Path(os.path.realpath(__file__)).parent.joinpath(specdir)
    env = Environment(
        loader=FileSystemLoader(templatedir),
        autoescape=select_autoescape(),
        keep_trailing_newline=True,
    )
    template = env.get_template("nautilus-avinfo.spec.jinja")

    for param in params:
        content = template.render(asdict(param))
        specfile = templatedir.joinpath(param.filename)
        with open(specfile, "w") as f:
            f.write(content)
