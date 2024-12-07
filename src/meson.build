
subdir('kaitai')
subdir('dat')
subdir('tileset')
subdir('libgrp')
subdir('iscript')

starformat_lib_sources = files(
	'Casc.cpp',
	'FileUtil.cpp',
	'Palette.cpp',
	'PngExporter.cpp',
	'Chk.cpp',
	'Font.cpp',
	'Grp.cpp',
	'Widgets.cpp',
	'DataChunk.cpp',
	'Hurricane.cpp',
	'Pcx.cpp',
	'Scm.cpp',
	'Storm.cpp',
	'Dds.cpp',
	'Breeze.cpp',
	'Smacker.cpp',
	'Converter.cpp',
	'Storage.cpp',
	'PaletteImage.cpp',
	'Color.cpp',
	'Wav.cpp',
	'Palette2D.cpp',
	'StringUtil.cpp',
	'platform.cpp',
	'UIConsole.cpp',
	'pacman.cpp',
	'FileNotFoundException.cpp',
	'PortraitsConverter.cpp',
	'GraphicsConverter.cpp',
	'SfxConverter.cpp',
	'NoValidPaletteException.cpp',
	'AbstractPalette.cpp',
	'Bootstrap.cpp',
	'PaletteConverter.cpp',
	'DatConverter.cpp',
	'TilesetConverter.cpp',
	'CampaignConverter.cpp',
	'WidgetsConverter.cpp',
	'GameUIConverter.cpp',
	'CursorConverter.cpp'
	)

# this option is needed for kaitai binary parser
add_project_arguments('-DKS_STR_ENCODING_NONE', language: 'cpp')

inc = include_directories('.')

libstarformat = static_library('starformat',
	starformat_lib_sources,
	startool_kaitai_sources,
	startool_dat_sources,
	startool_tileset_sources,
	libgrp_sources,
	iscript_sources,
	include_directories : config_incdir,
	dependencies : [casc_dep, storm_dep, zlib_dep, png_dep, imagemagickpp_dep, log4cxx_dep, bzip2_dep, nlohmann_json_dep, iconv_lib],
	install : true)

libstarformat_dep = declare_dependency(include_directories : [config_incdir, inc],
  link_with : libstarformat)



	

