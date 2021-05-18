# KernelKeylogger

# Project
  The goal of this Keylogger is to be able to intercept scanCodes which represents the current key that had been pressed. The advantage of this keylogger is that, it lies down in the kernel mode, to be more precise above a filter driver(FiDO) which in our case is the **KeybordClass0**. 
  
  
# How the hell it works? 
So basically, we have several functions in the **ntddk** module which give us the option to subvert devices and hook to them if we want. But to be more abstract, it work's like that: 
  
**Keyboard device stack before intjecting our filtter:** 

                 ...
                  v
                Filter(1) 
                  v
                Filter(2)
                  v
                Filter(3)
                  v
                 ...
                  v
        KBDCLASS(Keyboard Device)
                  v
              PS/2(Driver)
                  v
                 Bus
                 
As you can see this is the normal view of the ***DeviceStack*** which got builded at the start. Umm what does the normal means you might ask??, "normal" means that there are not extrenal malicious FiDO(Filter Device Drivers) which might be attached. So, what is going to happen if we, as malware writters will attach our own Device to that device stack? 

**Keyboard device stack after device had been attached:**

        Malicious keyboard device
                  v
               Filter(1) 
                  v
               Filter(2)
                  v
               Filter(3)
                  v
                 ...
                  v
        KBDCLASS(Keyboard Device)
                  v
              PS/2(Driver)
                  v
                 Bus
                 
It might look fine, and unsuspicious well..., if you have some experience with writting Windows kernel drivers you know that there is structure which call IRP(I/O request package) the name speakד by itself, drivers are able to communicate with these type of structures inorder to transfer data bettwen different Devices/Ports/etc. FiDO(Filter Device Drivers) transfers irp's permanently, each irp transfer from top to bottom - **FIFO**. So that, we can intercept each IRP packeage and extract data; in our case ScanCode.


# ToDo

* Extract scan codes to a file.
* Read scan codes from the exported file.
