import asyncio
import signal

MAX_SOCKETS = 50
DELAY = 0.03

async def connect_socket(i):
    try:
        reader, writer = await asyncio.open_connection("127.0.0.1", 4444)
        print("Connected!")
        writer.write("PASS 123\r\n".encode())
        await asyncio.sleep(DELAY)
        writer.write(f"NICK users{i}\r\n".encode())
        await asyncio.sleep(DELAY)
        writer.write(f"USER a{i} 0 * a\r\n".encode())
        await asyncio.sleep(DELAY)
        writer.write("JOIN #general\r\n".encode())
        await asyncio.sleep(DELAY)
        await writer.drain()
        i = 0
        while i < MAX_SOCKETS:
            # data = await reader.read(1024)
            # if not data:
            #     break
            # response = data.decode()
            # print(f"Received: {response}")

            await asyncio.sleep(DELAY)
            writer.write(f"PRIVMSG #general :A7san Server Fl3alam{i}\r\n".encode())
            await writer.drain()
            i += 1
        i = 0
        while i < MAX_SOCKETS:
            await asyncio.sleep(DELAY)
            writer.write(f"PART #general :finiched{i}\r\n".encode())
            await writer.drain()
            i += 1
    except asyncio.CancelledError:
        print(f"Task {i} was cancelled.")
    except Exception as e:
        print(f"Error: {e}")

async def main():
    tasks = []
    for i in range(MAX_SOCKETS):
        task = asyncio.create_task(connect_socket(i))
        tasks.append(task)
    try:
        await asyncio.gather(*tasks)
    except asyncio.CancelledError:
        print("Main task was cancelled.")

async def run_main():
    try:
        await main()
    except asyncio.CancelledError:
        print("Main task was cancelled.")

def signal_handler(signal, frame):
    print("Ctrl+C pressed. Cleaning up...")
    for task in asyncio.all_tasks():
        task.cancel()

if __name__ == "__main__":
    signal.signal(signal.SIGINT, signal_handler)
    asyncio.run(run_main())