# Hardware Encryption Key
My C implementation of a hardware encryption key in the form of an arduino nano. Interfaceable with
a usb attached to an arduino nano board. When encrypting a file, the file is encrypted with a true randomly generated key straight from the microcontroller. The key and initialation vector are placed at the beginning of the encrypted file. A second encryption for the key and iv are still being worked on so that the encrypted file can only be decrypted by the microcontroller with unique key saved in its memory. This is likely to be an asymmetric password encryption I just need to find the one that I find most suitible.  
Credit to [kokke][1] for tiny AES written in C.

[1]: https://github.com/kokke/tiny-AES-c