# KernelKeylogger

# Project
  The goal of this Keylogger is to be able to intercept scanCodes which represents the current key that had been pressed. The advantage of this keylogger is that, it lies down in the kernel mode, to be more precise above a filter driver(FID) which in our case is the **KeybordClass0**. 
  
  
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
                 
As you can see this is the normal view of the ***DeviceStack*** which got builded at the start. Umm what does the normal means you might ask??, "normal" means that there are not extrenal malisious FiDO(Filter Device Objcet) which might be attached. So, what is going to happen if we, as malware writters will attach our own Device to that device stack? 

**Keyboard device stack after device had been attached:**
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
