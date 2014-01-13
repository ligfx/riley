srcdir = '.'
blddir = '.'

def configure(conf):
    conf.find_program('clang', var='CC')
    conf.load('compiler_c')
    conf.env.append_unique('CFLAGS', ['-std=c99', '-Wall', '-Werror' ])
    conf.check_cfg(package='glib-2.0', args='--cflags --libs', uselib_store='glib') 
    conf.check_cfg(package='cairo', args='--cflags --libs', uselib_store='cairo')

def options(opt):
    opt.load('compiler_c')

def build(bld):
    bld.program(source='c16topng.c', target='c16topng', use='riley riley-cairo glib cairo')
    bld.shlib(source='riley.c', target='riley')
    bld.shlib(source='cairo.c', target='riley-cairo', use='riley cairo')
