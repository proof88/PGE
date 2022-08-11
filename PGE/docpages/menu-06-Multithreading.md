\page multithreading Multithreading

Note that anytime you want to use any of the tools mentioned on this page, it is highly recommended to make a **debug build** of your application you want to check.

[TOC]

\section multithreading_windows Tools for Windows

Unfortunately, Valgrind is not available for Windows.  
However, there are multiple tools for checking thread behavior for Windows.

There is an official extension for VS2022, **Concurrency Visualizer**:  
https://marketplace.visualstudio.com/items?itemName=Diagnostics.DiagnosticsConcurrencyVisualizer2022#overview  
https://docs.microsoft.com/en-us/visualstudio/profiling/common-patterns-for-poorly-behaved-multithreaded-applications?view=vs-2022

**Intel Inspector** can pinpoint data race, memory leak, etc:  
https://www.intel.com/content/www/us/en/developer/articles/tool/oneapi-standalone-components.html#inspector  
https://www.intel.com/content/www/us/en/developer/tools/oneapi/inspector.html#gs.8v7hpe

And also in Windows SDK, there is the **Application Verifier**:  
https://docs.microsoft.com/en-us/windows-hardware/drivers/devtest/application-verifier#installing-appverifier  
I'm not sure if I managed to use this tool properly though because it always generated no errors or warnings during checking my application even when I intentionally made some threading mistake.  
Don't forget that some exceptions are expected to be thrown when this tool is enabled, you need to set your debugger to handle those as second-chance, as described here:  
https://docs.microsoft.com/en-us/windows-hardware/drivers/devtest/application-verifier-debugging-application-verifier-stops

\section multithreading_valgrind Valgrind

Valgrind is my favourite tool to check thread related issues under Linux (and it is also useful for memory leak/corruption check).  
Understanding the output of Valgrind is not topic of this page.  
Read the [QuickStart Guide](https://valgrind.org/docs/manual/QuickStart.html) and the [Manual](https://valgrind.org/docs/manual/manual.html).

\subsection multithreading_valgrind_memcheck Memory Check

Example use for memory leak or corruption checking:
```  
valgrind --tool=memcheck --leak-check=full --leak-check-heuristics=all --trace-children=yes --track-origins=yes -s <binary_path>
```

**Suppression File**  
If you see some irrelevant complains, like about some conditional jumps based on uninitialized variables within a linux ld lib, you can add such irrelevant complains to suppression file.  
To ask valgrind to generate suppressions for you based on the detected errors, run it with adding **--gen-suppressions=all** option, and then you can copy those suppressions into a suppression file.  
And then you can simply re-run Valgrind using that suppression file by adding the **--suppressions=yourSuppressionFile.supp** option.

**In Case of Problem Related to strlen() or index()**  
However, when I ran it, it displayed complain about not finding strlen() function in the host machine's ld-linux.so.2 file. This is mandatory for the memcheck tool.  
Although it recommended adding glibc-debuginfo module as a short-term solution, it didn't help me!

**Workaround**  
I google'd around and understood this problem is present for like ~10 years already.  
If you can install packages then do that as explained on the internet.  
However, I was working on some machine where I was not allowed to install any package, so I had to rebuild Valgrind with a patch.  
This patch was made by someone who uploaded it to the internet:  
https://bugsfiles.kde.org/attachment.cgi?id=82043  
This patch disables the check for strlen() presence in the abovementioned shared libraries.

For better understanding the problem, you can read the comments in the bugreport:  
https://bugs.kde.org/show_bug.cgi?id=286864

To apply the patch, obviously we need to download and build Valgrind with the patch.

**Download Valgrind**  
Official site of Valgrind is this:  
https://valgrind.org/downloads/repository.html

It tells you that source code can be cloned like:
```
git clone git://sourceware.org/git/valgrind.git
```

The problem is that it simply timed out for me, maybe because of some firewall setting with that computer I tried it on.  
You can try alternate git server as stated on the site.

So I rather downloaded it from the official site as a package:  
https://valgrind.org/downloads/current.html

**Applying the Patch**  
You can download the patch and patch with the linux patch command and hope it is still compatible with the source code you've downloaded, or edit the coregrind/m_redir.c file manually as the patch would do. I did the latter.

Hint: not only the strlen() check but also the index() check should be also disabled! So edit the source code with this in mind!

Hint: I've noticed that in the latest version of Valgrind there is already a macro controlling this, if you look closely at the source code you will see it anyway!

**Building Valgrind**  
Now you need to follow the instructions as stated on the official site or in the README file:

```
cd valgrind-3.18.1
./autogen.sh
./configure --prefix=<PUT THE DIRECTORY WHERE YOU WANT TO BUILD YOUR CUSTOM VALGRIND>
make
make install
```

\subsection multithreading_valgrind_helgrind Helgrind Thread Checker

To debug threading related issues, the helgrind tool can be used, like:
```
valgrind --tool=helgrind --trace-children=yes --read-var-info=yes -s <binary_path>
```

\subsection multithreading_valgrind_drd DRD Thread Checker

This is also a Valgrind tool, to debug threading related issues:
```
valgrind --tool=drd --read-var-info=yes -s <binary_path>
```

\subsection multithreading_valgrind_further Further Info

Note that in some cases, Helgrind may show wrong errors if you use C++11 condition variables.  
Such wrong errors can be displayed as:  
either like "mutex is locked simultaneously by two threads"  
or like "Thread ... unlocked lock at ... currently held by thread ...".  
To solve this issue, you need to [apply patch not yet officially released](https://bugsfiles.kde.org/attachment.cgi?id=143599).  
[This is the related discussion](https://www.mail-archive.com/kde-bugs-dist@kde.org/msg622117.html), and [this is the ticket for it](https://bugs.kde.org/show_bug.cgi?id=445504).

Also it is good to know that proper behavior for Helgrind or DRD or even [ThreadSanitizer](https://github.com/google/sanitizers/wiki/ThreadSanitizerCppManual) requires 2 macros to be defined properly:  
**_GLIBCXX_SYNCHRONIZATION_HAPPENS_BEFORE**  
**_GLIBCXX_SYNCHRONIZATION_HAPPENS_AFTER**  
Details can be found in the manual of the tools, you can also [read about it here](https://gcc.gnu.org/onlinedocs/libstdc++/manual/debug.html), in the "Data Race Hunting" section.
