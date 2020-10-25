# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

import datetime
import os

# -- Check and build on readthedocs ---------------------------------------
if os.environ.get('READTHEDOCS', 'Not yet!') == 'True':
    import subprocess
    subprocess.call('doxygen')

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Ground Floor'
author = 'spky'
copyright = f'{datetime.date.today().year}, {author}'

# The full version, including alpha/beta/rc tags
release = 'v0.0.0'

today_fmt = '%Y-%m-%d'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    'sphinx.ext.autodoc',
    'breathe',
]

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']


# -- Options for breathe (and doxygen) ------------------------------------

breathe_default_project = 'GroundFloor'
breathe_projects = {
    breathe_default_project: '_build/doxyxml/'
}
breathe_default_members = ('members', 'private-members', 'undoc-members')


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'alabaster'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
html_theme_options = {
    'sidebar_includehidden': False,
    'show_related': True
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# # The name of an image file (relative to this directory) to place at the top
# # of the sidebar.
html_logo = '_static/logo.png'
