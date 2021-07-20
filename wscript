# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('flood-wup', ['core'])
    module.source = [
        'model/flood-wup.cc',
        'model/flood-wup-main.cc'
        'helper/flood-wup-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('flood-wup')
    module_test.source = [
        'test/flood-wup-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/flood-wup-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'flood-wup'
    headers.source = [
        'model/flood-wup.h',
        'model/flood-wup-main.h',
        'helper/flood-wup-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

