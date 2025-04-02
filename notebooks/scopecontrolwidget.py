import pymmcore
import os.path
import time
from pylablib.devices import Thorlabs
import pyvisa

import nidaqmx
import nidaqmx.system
from nidaqmx.types import CtrTime

import threading
from ipywidgets import Layout, Button, Box, Checkbox, FloatText, Textarea, Output, Dropdown, Label, IntSlider, GridspecLayout, Tab, Text, jslink
from IPython.display import display

import sys
sys.path.append(r"C:/Users/TCSPC/Documents/GitHub/davy-jones/")
from davy_jones import DeepSee

# initialize MM-controlled devices

mm_dir = r"C:\Program Files\Micro-Manager-2.0"
mmc = pymmcore.CMMCore()
print( pymmcore.CMMCore().getAPIVersionInfo() )

mmc.setDeviceAdapterSearchPaths([mm_dir])
mmc.loadSystemConfiguration(os.path.join(mm_dir, "MMConfig_xyz_shutter_filter2.cfg"))

print("Loaded devices:", mmc.getLoadedDevices())

def displayDeviceProperties(devicename):
    for prop in mmc.getDevicePropertyNames(devicename):
        print(prop, ": ",  mmc.getProperty(devicename, prop))


# initialize NI DAQ

system = nidaqmx.system.System.local()
print(system.driver_version)
for device in system.devices:
    print(device)

    
# initialize kinesis / APT devices
    
kinesis_devices = Thorlabs.list_kinesis_devices()
sync_stage, hwp_stage = kinesis_devices[0], kinesis_devices[1]
hwp_ID, sync_ID = kinesis_devices[1][0], kinesis_devices[0][0]
hwp_scale = "stage"
sync_scale = 1920
print("Kinesis devices:", kinesis_devices)


# initialize laser

rm = pyvisa.ResourceManager()
resource_list = rm.list_resources()
print("pyVISA resources:", resource_list)
insight_laser = rm.open_resource(resource_list[-1])
insight_laser.baud_rate = 115200
insight_laser.write_termination = '\r'

# 

hwp_mutex = threading.Lock()
sync_mutex = threading.Lock()
shutter_mutex = threading.Lock()
filtwhl_mutex = threading.Lock()
xystage_mutex = threading.Lock()
zpiezo_mutex = threading.Lock()
laser_mutex = threading.Lock()

def hwp_home():
    global hwp_mutex

    hwp_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(hwp_ID, scale=hwp_scale) as hwp_stage:
            hwp_stage.home()
            hwp_stage.wait_for_home()
            hwp_stage.move_to(0)
    finally:
        hwp_mutex.release()
        
def sync_home():
    global sync_mutex

    sync_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(sync_ID, scale=sync_scale) as sync_stage:
            sync_stage.home()
            sync_stage.wait_for_home()
            sync_stage.move_to(0)
    finally:
        sync_mutex.release()

hwp_home()
sync_home()

def HWP_set_pos(target):
    global hwp_mutex

    hwp_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(hwp_ID, scale=hwp_scale) as hwp_stage:
            hwp_stage.move_to(target)
            hwp_stage.wait_for_stop()
    finally:
        hwp_mutex.release()
        
def HWP_get_pos():
    global hwp_mutex

    hwp_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(hwp_ID, scale=hwp_scale) as hwp_stage:
            pos = hwp_stage.get_position()
    finally:
        hwp_mutex.release()
    
    return pos

def SYNC_set_pos(target):
    global hwp_mutex

    sync_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(sync_ID, scale=sync_scale) as sync_stage:
            sync_stage.move_to(target)
            sync_stage.wait_for_stop()
    finally:
        sync_mutex.release()
        
def SYNC_get_pos():
    global sync_mutex

    sync_mutex.acquire()
    try:
        with Thorlabs.KinesisMotor(sync_ID, scale=sync_scale) as sync_stage:
            pos = sync_stage.get_position()
    finally:
        sync_mutex.release()
    
    return pos

def HWP_value_change(change):
    if (change["new"] >= 0) and (change["new"] <= 359.99):        
        HWP_set_pos(change["new"])
        
def SYNC_value_change(change):
    if (change["new"] >= 0) and (change["new"] <= 359.99):        
        SYNC_set_pos(change["new"])
        
def laser_shutter(change):
    global laser_mutex
    
    laser_mutex.acquire()
    
    shutter_state = int(insight_laser.query_ascii_values(r"SHUTter?", converter="s")[0])
    if shutter_state == 0:
        # open shutter
        insight_laser.write_ascii_values(r"SHUTter 1", "")
    elif shutter_state == 1:
        # close shutter
        insight_laser.write_ascii_values(r"SHUTter 0", "")
    else:
        print("Error: Unrecognized shutter state.")
    
def laserWL_set(change):
    insight_laser.write_ascii_values(r"WAV " + str(change), "")
    
def laserWL_value_change(change):
    if (change["new"] >= 690) and (change["new"] <= 1300):        
        laserWL_set(change["new"])
        
def shutter_value_change(change):
    if change["new"]:
        mmc.setShutterOpen(True)
    else:
        mmc.setShutterOpen(False)
        
def filtwhl_value_change(change):
    mmc.setStateLabel("Wheel-A", "Filter-" + str(change["owner"].index))
    
def filtwhl_get_pos():
    currpos = mmc.getStateLabel("Wheel-A")
    currindex = wheel_values.index(currpos)
    return currindex

def XYZ_get_pos():
    XYpos = mmc.getXYPosition()
    X, Y = tuple(XYpos)
    Z = mmc.getPosition()
    return X, Y, Z

def X_set_pos_abs(new_X):
    XYpos = mmc.getXYPosition()
    X, Y = tuple(XYpos)
    mmc.setXYPosition(new_X["new"],Y)
    
def Y_set_pos_abs(new_Y):
    XYpos = mmc.getXYPosition()
    X, Y = tuple(XYpos)
    mmc.setXYPosition(X,new_Y["new"])
    
def Z_set_pos_abs(new_Z):
    print("newZ = ", new_Z["new"])
    mmc.setPosition(new_Z["new"])
    # mmc.setPosition(62)
    
# def X_set_pos_rel(factor=1):
#     dX = factor * form_items[7].children[1].value
#     X, Y = tuple(mmc.getXYPosition())
#     mmc.setXYPosition(X+dX,Y)
    
# def Y_set_pos_rel(factor=1):
#     dY = factor * form_items[8].children[1].value
#     X, Y = tuple(mmc.getXYPosition())
#     mmc.setXYPosition(X,Y+dY)
    
# def Z_set_pos_rel(factor=1):
#     dZ = factor * form_items[9].children[1].value
#     X, Y = tuple(mmc.getXYPosition())
    
#     Z = mmc.getPosition()
#     mmc.setPosition(Z+dZ)
    
def X_set_pos_rel(dX):
    X, Y = tuple(mmc.getXYPosition())
    mmc.setXYPosition(X+dX,Y)
    
def Y_set_pos_rel(dY):
    X, Y = tuple(mmc.getXYPosition())
    mmc.setXYPosition(X,Y+dY)
    
def Z_set_pos_rel(dZ):
    Z = mmc.getPosition()
    mmc.setPosition(Z+dZ)
    
filter_list = ["Empty", "460/50 NADH", "560/40", "Empty", "582/64 FAD2", "535/30 GFP/YFP", "530/35 GFP/YFP", "650/100 Mitotracker", "Unknown", "Empty"]
wheel_values = mmc.getStateLabels("Wheel-A")

form_item_layout = Layout(
    display='flex',
    flex_flow='row',
    justify_content='space-between'
)

X, Y, Z = XYZ_get_pos()

form_items = [
    Box([Label(value="HWP"), FloatText(HWP_get_pos()), FloatText()], layout=form_item_layout),
    Box([Label(value="SYNC"), FloatText(SYNC_get_pos()), FloatText()], layout=form_item_layout),
    Box([Label(value="Shutter"), Checkbox(), Checkbox()], layout=form_item_layout),
    Box([Label(value='Emission filter'), Dropdown(options=filter_list), Dropdown(options=filter_list)], layout=form_item_layout),
    Box([Label(value="X"), FloatText(X), FloatText()], layout=form_item_layout),
    Box([Label(value="Y"), FloatText(Y), FloatText()], layout=form_item_layout),
    Box([Label(value="Z"), FloatText(Z), FloatText()], layout=form_item_layout),
    Box([Label(value="dX"), FloatText(60), Button(description="<"), Button(description=">")], layout=form_item_layout),
    Box([Label(value="dY"), FloatText(60), Button(description="v"), Button(description="^")], layout=form_item_layout),
    Box([Label(value="dZ"), FloatText(0.5), Button(description="v"), Button(description="^")], layout=form_item_layout),
    Box([Label(value="Laser WL TARGET"), FloatText(), FloatText()], layout=form_item_layout),
    Box([Label(value="Laser MTRPOS TARGET"), FloatText(), FloatText()], layout=form_item_layout),
]

form_items[0].children[1].observe(HWP_value_change, names="value")
form_items[1].children[1].observe(SYNC_value_change, names="value")
form_items[2].children[1].observe(shutter_value_change, names="value")
form_items[3].children[1].observe(filtwhl_value_change, names="value")
form_items[4].children[1].observe(X_set_pos_abs, names="value")
form_items[5].children[1].observe(Y_set_pos_abs, names="value")
form_items[6].children[1].observe(Z_set_pos_abs, names="value")
form_items[7].children[2].on_click(lambda f: X_set_pos_rel(-form_items[7].children[1].value))
form_items[7].children[3].on_click(lambda f: X_set_pos_rel(form_items[7].children[1].value))
form_items[8].children[2].on_click(lambda f: Y_set_pos_rel(-form_items[8].children[1].value))
form_items[8].children[3].on_click(lambda f: Y_set_pos_rel(form_items[8].children[1].value))
form_items[9].children[2].on_click(lambda f: Z_set_pos_rel(-form_items[9].children[1].value))
form_items[9].children[3].on_click(lambda f: Z_set_pos_rel(form_items[9].children[1].value))
form_items[10].children[1].observe(laserWL_value_change, names="value")


def update_status_bg_t1(form_items):
    """
    Update system status in the background
    """

    while True:
        time.sleep(0.1)
        form_items[0].children[-1].value = HWP_get_pos()
        form_items[1].children[-1].value = SYNC_get_pos()
        
def update_status_bg_t2(form_items):
    """
    Update system status in the background
    """

    while True:
        time.sleep(0.1)
        # form_items[0].children[-1].value = HWP_get_pos()
        # form_items[1].children[-1].value = SYNC_get_pos()
        form_items[2].children[-1].value = mmc.getShutterOpen()
        form_items[3].children[-1].index = filtwhl_get_pos()
        X, Y, Z = XYZ_get_pos()
        form_items[4].children[-1].value = X
        form_items[5].children[-1].value = Y
        form_items[6].children[-1].value = Z

t1 = threading.Thread(target=update_status_bg_t1, args=(form_items,))
t2 = threading.Thread(target=update_status_bg_t2, args=(form_items,))

form = Box(form_items, layout=Layout(
    display='flex',
    flex_flow='column',
    border='solid 2px',
    align_items='stretch',
    width='100%'
))

tab = Tab()
tab.children = [form, Text(description="name")]
tab.titles = ["Preview", "Acquisition"]

# display(form)
display(tab)

t1.start()
t2.start()