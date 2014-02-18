=======================================================
BUGS IN RECORDING: 
- Will not minimize
- BitRate settings are not used when recording

=======================================================
BUGS IN PLAYER:


=======================================================

The player has a lot of control, so it will be broken
down into different components.  They will each be closely
tied, but will be separate.

The actual player, then, will just be a main window and
a tab control.  Inside the tab control will be the different
"tabs".  They will be a separate main window that will be
owned by the different components.

The player will first go through the plugin initialization
routine.  

- Obtain a LLPluginEnumerator address through some sort of 
	discovery.
- Call this function and for each plugin, save its information
    in a LL_PLUGIN_INFORMATION struct and add it to the list.
- Now we have all the plugins - create them all by calling
    their notification function with the LL_PLUGIN_ATTACH
    message.  This will also return more specific information
    about the plugin that will be saved.
    
Nov. 5, 2001
Since the plugins need to be notified of a bunch of stuff,
the Core will need to be able to send notifications to the
plugins too.

