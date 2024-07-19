Driver library for custom virtual interrupt board designed for arduino uno

Personal project of mine when I realized there are only 2 external interrupts on an arduino uno. Designed a digital logic board to serialize input from 32 external devices (buttons etc) into a single interrupt pin, hence there are 32 'virtual' interrupts

ideally, these act as normal interrupts, just requiring an additional layer of software abstraction to function.

In all likelihood you are probably better off upgrading to an arduino mega with more built in interrupt pins, since without an economy of scale im going to be using relatively large SMD logic chips on the board. Doesnt lend itself to small size. Still, feel free to use this, and assemble a board (when all this is done)