# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('wake-up-radio', ['core'])
    module.source = [
        'model/wur-ppdu.cc',
	'model/wur-common-channel.cc',
	'model/wur-common-phy.cc',
	'model/wur-common-ppdu.cc',
	'model/wur-common-psdu.cc',
	'model/wur-common-net-device.cc',
	'model/wur-shared-mac.cc',
	'model/wur-shared-mac-dummy-impl.cc',
	'model/wur-common-phy-dummy-impl.cc',
	'model/wur-common-net-device-dummy-impl.cc',
        'model/radio-energy-model.cc',
        'model/main-radio-energy-model.cc',
        'model/wur-radio-energy-model.cc',
        'model/flood-wakeup-packet.cc'
        ]

    module_test = bld.create_ns3_module_test_library('wake-up-radio')
    module_test.source = [
        #'test/wake-up-radio-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/wake-up-radio-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'wake-up-radio'
    headers.source = [
        'model/wur-ppdu.h',
	'model/wur-common-channel.h',
	'model/wur-common-ppdu.h',
	'model/wur-common-psdu.h',
	'model/wur-common-phy.h',
	'model/wur-common-net-device.h',
	'model/wur-shared-mac.h',
	'model/wur-shared-mac-dummy-impl.h',
	'model/wur-common-phy-dummy-impl.h',
	'model/wur-common-net-device-dummy-impl.h',
        'model/radio-energy-model.h',
        'model/main-radio-energy-model.h',
        'model/wur-radio-energy-model.h',
        'model/flood-wakeup-packet.h'
       ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()

