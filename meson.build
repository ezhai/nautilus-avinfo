project('nautilus-avinfo', 'c',
    default_options : ['buildtype=release', 'prefix=/usr'],
    version : '0.1.0',
)

## DIRECTORIES
prefix = get_option('prefix')
localedir = join_paths(prefix, get_option('localedir'))
libdir = join_paths(prefix, get_option('libdir'))
extensiondir = join_paths(libdir, 'nautilus', 'extensions-4')

## CONFIGURATION
conf_data = configuration_data()
conf_data.set('DEBUG', 0)
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

## TARGET
shared_library('nautilus-avinfo', src,
    include_directories : include_directories('.'),
    dependencies : deps,
    name_prefix: '',
    install : true,
    install_dir : extensiondir,
)
