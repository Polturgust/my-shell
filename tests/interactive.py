#!/usr/bin/env python3
"""Drive 42sh through a pseudo-terminal and check the interactive features:
line editing, history, completion, Ctrl+C / Ctrl+Z, jobs and ! expansion."""
import os, pty, re, select, sys, tempfile, time

SHELL = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else "./42sh")
ANSI = re.compile(r"\x1b\[[0-9;?]*[A-Za-z]|\r")
PROMPT = "➜ "
results = []


class Term:
    def __init__(self, home):
        self.pid, self.fd = pty.fork()
        if self.pid == 0:
            os.chdir(home)
            os.execve(SHELL, [SHELL], {"PATH": "/usr/bin:/bin", "HOME": home,
                                       "TERM": "xterm", "USER": "test"})
        self.out = b""

    def read(self, timeout=0.6):
        end = time.time() + timeout
        while time.time() < end:
            r, _, _ = select.select([self.fd], [], [], 0.05)
            if r:
                try:
                    data = os.read(self.fd, 65536)
                except OSError:
                    return
                if not data:
                    return
                self.out += data
                end = max(end, time.time() + 0.15)

    def send(self, keys, wait=0.6):
        self.out = b""
        os.write(self.fd, keys.encode() if isinstance(keys, str) else keys)
        self.read(wait)
        return self.text()

    def text(self):
        return ANSI.sub("", self.out.decode("utf-8", "replace"))


def check(name, cond, detail=""):
    results.append(cond)
    print(("ok   " if cond else "FAIL ") + name + ("" if cond else "\n     " + repr(detail[-300:])))


def last_line_before_prompt(text):
    return text.split(PROMPT)[0]


home = tempfile.mkdtemp()
os.makedirs(os.path.join(home, "projects/alpha"))
os.makedirs(os.path.join(home, "pictures"))
open(os.path.join(home, "notes.txt"), "w").write("x\n")
t = Term(home)
t.read(1.5)
check("prompt shown", PROMPT in t.text(), t.text())

out = t.send("echo hello\r")
check("run a command", "hello" in out, out)

out = t.send("echo wrld\x1b[D\x1b[D\x1b[Do\r")
check("left arrow then insert", "world" in out, out)

out = t.send("cho abc\x01e\x05!\r")
check("Ctrl+A / Ctrl+E", "abc!" in last_line_before_prompt(out.split("\n", 1)[1]), out)

out = t.send("echo bc\x1b[H\x1b[C\x1b[C\x1b[C\x1b[C\x1b[Ca\x1b[F!\r")
check("Home / End keys", "abc!" in last_line_before_prompt(out.split("\n", 1)[1]), out)

out = t.send("echo one two three\x17\x17four\r")
check("Ctrl+W deletes words", "one four" in out, out)

out = t.send("echo start\x15echo replaced\r")
check("Ctrl+U clears to start", "replaced" in out and "start" not in out.split("\n")[-2], out)

out = t.send("\x1b[A\r")
check("up arrow recalls last command", out.count("replaced") >= 2, out)

t.send("echo first-entry\r")
t.send("echo second-entry\r")
out = t.send("\x1b[A\x1b[A\x1b[B\r")
check("up, up, down", "second-entry" in out, out)

out = t.send("\x12first\r")
check("Ctrl+R reverse search", "first-entry" in out, out)

out = t.send("!!\r")
check("!! repeats last command", "first-entry" in out, out)

out = t.send("ec\t")
check("Tab completes a command", "echo " in out, out)
t.send("\x15")

out = t.send("ls pro\t")
check("Tab completes a folder with /", "projects/" in out, out)
out = t.send("\t")
check("Tab inside folder", "projects/alpha/" in out, out)
t.send("\x15")

out = t.send("ls p\t", 0.8)
check("Tab lists choices", "pictures/" in out and "projects/" in out, out)
t.send("\x15")

out = t.send("cat no\t")
check("Tab completes a file with space", "notes.txt " in out, out)
out = t.send("\r")
check("completed command runs", "x" in out, out)

out = t.send("echo $HO\t")
check("Tab completes $variables", "$HOME" in out, out)
t.send("\x15")

out = t.send("sleep 5\r", 0.4)
out = t.send("\x03", 0.8)
out = t.send("echo alive\r")
check("Ctrl+C stops the command, shell survives", "alive" in out, out)

out = t.send("typing\x03")
out = t.send("echo after-cancel\r")
check("Ctrl+C cancels the line", "after-cancel" in out and "typing" not in out, out)

t.send("sleep 30\r", 0.4)
out = t.send("\x1a", 0.8)
check("Ctrl+Z suspends", "Suspended" in out, out)
out = t.send("jobs\r")
check("jobs lists it", "Suspended" in out and "sleep 30" in out, out)
out = t.send("bg\r")
check("bg resumes in background", "sleep 30 &" in out, out)
out = t.send("fg\r", 0.6)
out = t.send("\x03", 0.8)
out = t.send("jobs\r")
check("fg then Ctrl+C ends it", "sleep" not in last_line_before_prompt(out), out)

out = t.send("sleep 0.3 &\r", 0.5)
check("& prints job number", re.search(r"\[1\] \d+", out) is not None, out)
time.sleep(0.6)
out = t.send("\r")
check("Done notice", "Done" in out, out)

out = t.send("cat << EOF\rline1\rEOF\r")
check("heredoc at the prompt", "? " in out and "line1" in out, out)

long_cmd = "echo " + "x" * 150
out = t.send(long_cmd + "\r")
check("long line wraps and runs", "x" * 150 in out.replace("\n", ""), out)

out = t.send("cd projects\r")
out = t.send("\r")
check("prompt shows ~/projects", "~/projects" in out, out)

out = t.send("\x04", 0.8)
check("Ctrl+D exits", "exit" in out, out)

print(f"\n{sum(results)}/{len(results)} passed")
sys.exit(0 if all(results) else 1)
