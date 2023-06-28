import vlab
import os
import sys
import struct

import keystone.c66_cluster.defines
import keystone.compute_cluster.defines

keystone.c66_cluster.defines.MODEL_PARAMS['c66p']['cycle_time_ps'] = 200000
keystone.compute_cluster.defines.MODEL_PARAMS['c7x']['cycle_time_ps'] = 200000

print ( '# c66p cycle time per sec = ' + str(keystone.c66_cluster.defines.MODEL_PARAMS['c66p']['cycle_time_ps']));
print ( '# c7x  cycle time per sec = ' + str(keystone.compute_cluster.defines.MODEL_PARAMS['c7x']['cycle_time_ps']));

psdk_rtos_workarea = '/ti/j7presi/workarea'

testbench_args = '--testbench=' + psdk_rtos_workarea + '/tiovx/tiovx_dev/internal_docs/vlab_scripts/testbench.py'
mmc_args = '--mmc=platform.emmccard0:' + psdk_rtos_workarea + '/mmccard0.cfg'

#image = psdk_rtos_workarea + "/pdk/packages/ti/binary/csirx_capture_testapp/bin/simJ7/csirx_capture_testapp_mcu2_0_debug.xer5f"

vlab.load(u"keystone_scripts.j7es_ccs", args= [testbench_args, mmc_args, '--ddr0_size=0x80000000','--resolution=platform.dss.VP2:1280x800']  + __args__)
vlab.display_terminal(vlab.terminal.usart0);

print "# Loading mcu2_0 ..."
image = psdk_rtos_workarea + "/vision_apps/out/J7/R5F/SYSBIOS/debug/vx_app_tirtos_mcu2_0.out"
core = 'pulsar0_cr5f_0_proxy'
vlab.load_image(image, kind='elf', subject=core, load_symbols=True)

print "# Loading mpu1 ..."
image = psdk_rtos_workarea + "/vision_apps/out/J7/A72/SYSBIOS/debug/vx_app_tirtos_mpu1.out"
core = 'ca72_1_0_proxy'
vlab.load_image(image, kind='elf', subject=core, load_symbols=True)

_transmit_image_result = True
_transmit_image_count = 0

#def check_memory(bp):
#    global _transmit_image_result
#    global return_code
#
#    golden = "Validation test completed.\nResult: PASS\n"
#    PRINTF_BASE_ADDRESS = 0x80400000
#
#    try:
#        print "Breakpoint at", vlab.simulation_time()
#        length = str(vlab.read_memory(PRINTF_BASE_ADDRESS, 4, core=core))
#        length = struct.unpack("<I", length)[0]
#        memory = str(vlab.read_memory(PRINTF_BASE_ADDRESS + 4, length, core=core))
#        print memory
#        if memory.endswith(golden) and _transmit_image_result:
#             print "PASS\n"
#        else:
#            print "FAIL\n"
#            return_code = 1
#
#    except:
#        print "FAIL\n"
#        return_code = 1
#        raise
#
#    vlab.pause()

#def update_check_memory(value):
#    vlab.write_symbol("memory_check", value, core="ca72_1_0_proxy", space=None)

def transmit_image_result(success):
    global _transmit_image_result
    global _transmit_image_count
    _transmit_image_result = success
    _transmit_image_count = _transmit_image_count + 1

    if not _transmit_image_result:
        print "# ERROR in sending image #" + str(_transmit_image_count)
    else:
        print "# Sent image #" + str(_transmit_image_count)


def transmit_image_old(bp):
    global _transmit_image_result
    _transmit_image_result = False
    ppi_generator = vlab.get_instance("PPI_GENERATOR0")
    test_img = "PSP1Bordered_bmp_prog_planar_1920_1080.png"
    # parameter 1: image name, parameter 2: lines, parameter 3: columns
    # columns should be in term of pixel number
    ppi_generator.obj.generate_rgb_image(test_img, 240, 320)
    ppi_generator.obj.send_file(test_img, transmit_image_result)

def transmit_image(bp):
    global _transmit_image_result
    global lines
    global columns
    global vc
    _transmit_image_result = False
    ppi_generator = vlab.get_instance("PPI_GENERATOR0")
    test_img = "test.png"
    # parameter 1: image name, parameter 2: lines, parameter 3: columns
    # columns should be in term of pixel number
    ppi_generator.obj.generate_rgb_image(test_img, 240, 320)
    vc = 0
    ppi_generator.obj.reset_vc(vc)
    ppi_generator.obj.send_file(test_img, transmit_image_result)


print "# Adding breakpoint ..."
core = 'pulsar0_cr5f_0_proxy'
core = vlab.get_instance(core)
vlab.add_breakpoint(vlab.trigger.execute("App_sendFrame", core),
                    action=transmit_image, hidden=True)

#vlab.add_trace(subject="platform.csi_rx0_shim", sink=vlab.sink.vtf, verbose=False)
#vlab.add_trace(subject="platform.navssmcu_psilcfg", sink=vlab.sink.vtf, verbose=False)
#vlab.add_trace(subject="platform.navss512_udmap0", sink=vlab.sink.vtf, verbose=False)

print "# Running ..."
vlab.run();

