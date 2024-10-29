# This file is part of the MarineMindRobotics project.
#
# Copyright (C) 2024 MarineMindRobotics
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.

import os

license_header_cpp = """/*
 * This file is part of the MarineMindRobotics project.
 *
 * Copyright (C) 2024 MarineMindRobotics
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
"""

license_header_py = """# This file is part of the MarineMindRobotics project.
#
# Copyright (C) 2024 MarineMindRobotics
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
"""

ignore_dirs = ['aquabot-env-sirehna', 'include-ros', 'root', 'vrx']

file_extensions = ['.cpp', '.hpp', '.py']

def add_license_header(file_path):
    with open(file_path, 'r+') as file:
        content = file.read()
        if file_path.endswith('.py'):
            license_header = license_header_py
        else:
            license_header = license_header_cpp

        # Check if the header is already present
        if content.lstrip().startswith(license_header.strip()):
            return

        file.seek(0, 0)
        file.write(license_header + '\n' + content)

def should_ignore_dir(dir_name):
    return any(ignore in dir_name for ignore in ignore_dirs)

def process_files(root_dir):
    for subdir, dirs, files in os.walk(root_dir):
        if should_ignore_dir(subdir):
            continue
        for file in files:
            if any(file.endswith(ext) for ext in file_extensions):
                file_path = os.path.join(subdir, file)
                add_license_header(file_path)

if __name__ == "__main__":
    process_files('.')
