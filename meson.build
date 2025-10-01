project('nautilus-avinfo', 'c',
    default_options : ['buildtype=release', 'prefix=/usr'],
    version : '0.2.0',
)
add_project_arguments('-DG_LOG_DOMAIN="nautilus-avinfo"', language: 'c')
release = '1'

## PROGRAMS
python = find_program('python', 'python3', required: true)

## DIRECTORIES
prefix = get_option('prefix')
localedir = join_paths(prefix, get_option('localedir'))
libdir = join_paths(prefix, get_option('libdir'))
extensiondir = join_paths(libdir, 'nautilus', 'extensions-4')

## CONFIGURATION
conf_data = configuration_data()
conf_data.set_quoted('PACKAGE', meson.project_name())
conf_data.set_quoted('LOCALEDIR', localedir)
configure_file(
    output: 'config.h',
    configuration: conf_data,
)

## BUILD
src = files()
deps = [
    dependency('libnautilus-extension-4'),
    dependency('libavcodec'),
    dependency('libavformat'),
    dependency('libavutil'),
    dependency('threads'),
]

subdir('src')
subdir('po')

## TARGETS
shared_library('nautilus-avinfo', src,
    include_directories : include_directories('.'),
    dependencies : deps,
    name_prefix: '',
    install : true,
    install_dir : extensiondir,
)

run_target('nautilus-avinfo-uninstall',
    command : ['scripts/uninstall.sh', libdir, localedir],
)

run_target('rpm-spec',
    command : [python, '@CURRENT_SOURCE_DIR@/scripts/generate-rpm-spec.py', meson.project_version(), release, '@CURRENT_SOURCE_DIR@/pkg/rpm'],
)

run_target('pkgbuild',
    command : [python, '@CURRENT_SOURCE_DIR@/scripts/generate-pkgbuild.py', meson.project_version(), release, '@CURRENT_SOURCE_DIR@/pkg/aur'],
)
