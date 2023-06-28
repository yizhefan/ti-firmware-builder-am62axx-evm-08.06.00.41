# File Descriptor Exchange across Processes {#group_apps_basic_demos_app_arm_fd_exchange}

[TOC]

# Introduction

This application demonstrates mechanism by which memory buffers could be exchanged across different processes for achieving zero-copy semantics. This is important for the applications where copying massive amounts of data is not an option when data producers and consumers span multiple processes.

The following is the sequence of operations that happen in this demo.

NOTE: UNIX sockets have been specifially chosen as the IPC mechanism between the processes since it is the only way one can exchange the file descriptors across processes under linux (the message format has specific control fields that aid in passing the information through the linux kernel for the fd translation).

1. A process producing the data, Producer, is instantiated. This process starts a UNIX socket based server and waits for client connections. This is an iterative server and accepts only one client connection. This process must be the first one to start, otherwise, the Consumer process will fail during its connection attempt.
2. A process consuming the data, Consumer, is instantiated. This process initiates a client connection with the Producer process.
3. The Producer sends a HELLO CMD and waits for HELLO RSP from the Consumer. This is just a basic check. In practical applications, such an exchange could be used to piggy back any Producer/Consumer capability information (ex:- sensor information). In this demo it is just a customary greeting exchange.
4. The Producer allocates a predefined number of vxImage objects, exports the handles and meta information, translates the exported handles to file descriptors to be sent over to the Consumer process.
5. The Producer sends the vxImage meta information together with the file descriptors using the UNIX sockets. Since there are multiple objects to be transferred, the Producer need to inform the Consumer when to stop looking for the incoming object information. For this, the message carries a 'lastObject' flag to indicate if the current message is the last one of its kind carrying the object information. The Producer also writes a small pattern into the exported buffers. The pattern is generated using an identical computation from a seed. This computation and seed is known to both the Producer and Consumer processes. 
6. When the Consumer receives the message indicating object information, it extracts the file descriptors, decodes the meta information, creates a vxImage object, translates the file descriptor into virtual addresses (also called handles) and imports the handles into the newly created vxImage objects. The consumer verifies the pattern in the received buffers and flags if there is a mismatch.
7. Once the object exchange is complete, both sides will have two objects pointing to the same memory. Additional messages could be developed to exchange the exact buffer details based on the need. For example, when the Producer has written data to Object 'N', it can send the value 'N' over to the Consumer to inform that the data corresponding to object 'N' has been updated and is ready to be consumed. Once the Consumer is done consuming the data from object 'N', it can send a message back to the Producer saying thet the Producer can use object 'N' again.
8. This demo ends after the object exchange is done and both sides go ahead and release the vxImage objects.

# Supported plaforms

Platform  | Linux x86_64 | Linux+RTOS mode | QNX+RTOS mode | SoC
----------|--------------|-----------------|---------------|----
Support   | NO           | YES             |  YES          | J721e / J721S2 / J784S4

# Steps to run the application on J7 EVM

-# Build the application and related libraries as mentioned in \ref BUILD_INSTRUCTIONS
-# QNX-ONLY: Run the CPSW2G driver as shown below.
   \code
   io-pkt-v6-hc
   \endcode
-# Run the Producer executable. You can provide an optional command line option '-v' to see INFO messages. The command below puts the process in the background. This is only needed if the Consumer process is going to be launched in the same shell.
   \code
   ./vx_app_arm_fd_exchange_producer.out &
   \endcode
-# Run the Consumer executable. You can provide an optional command line option '-v' to see INFO messages
   \code
   ./vx_app_arm_fd_exchange_consumer.out
   \endcode
-# If all goes well, there should be no ERROR messages on the screen

