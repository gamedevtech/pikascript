WINDOWS
-------

One line install boot strap:
	
	powershell.exe -Command "(New-Object System.Net.WebClient).DownloadFile('http://nuedge.net/pikascript/install.bat','%TEMP%\install.bat')" && %TEMP%\install.bat


MAC / UNIX
----------

One line install boot strap:
	
	bash <(curl -s http://nuedge.net/pikascript/install.sh)

On Ubuntu you may have to first install curl, g++ and 32-bit libraries:

	sudo apt-get install curl g++ g++-multilib

Source distribution of PikaCmd compiles a 32-bit PikaScript target, also on 64-bit operating systems.

PikaScript can be compiled to 64-bit too, but doing so will change the size of integers from 32 to 64-bit.
Otherwise length(s) etc would not work properly. This means that bitwise operations might give different results than
in 32-bit PikaScript. E.g. ~0 will not return 4294967295, but 18446744073709551615.
