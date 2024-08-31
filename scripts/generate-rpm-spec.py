#!/bin/python3

import os
from pathlib import Path
from dataclasses import dataclass, asdict

from jinja2 import Environment, FileSystemLoader, select_autoescape

@dataclass
class Data:
    local: bool
    filename: str


if __name__ == "__main__":
    params = [
        Data(local=False, filename="nautilus-avinfo.spec"),
        Data(local=True, filename="nautilus-avinfo.local.spec")
    ]

    templatedir= Path(os.path.realpath(__file__)).parent.joinpath("../pkg/rpm/")
    env = Environment(
        loader=FileSystemLoader(templatedir),
        autoescape=select_autoescape(),
    )
    template = env.get_template("nautilus-avinfo.spec.jinja")

    for param in params:
        content = template.render(asdict(param))

        specfile = templatedir.joinpath(param.filename)
        with open(specfile, "w") as f:
            f.write(content)
