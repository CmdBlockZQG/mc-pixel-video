const readline = require('readline')
const { spawn } = require('child_process')
const fs = require('fs')

const rl = readline.createInterface({
    input: process.stdin
})

// java -Xmx2048M -Xms2048M -jar .\server.jar nogui
const server_process = spawn('java', ['-Xmx1024M', '-Xms1024M', '-jar', 'server.jar', 'nogui'])

server_process.stdout.on('data', data => {
    process.stdout.write(data)
})

let interval = 100
let timer = null
let currentFrame = 0
let frameCount = 0

rl.on('line', (s) => {
    if (s.indexOf('init') === 0) {
        frameCount = parseInt(s.split(' ')[1])
        console.log(`初始化 总帧数${frameCount}`)
        drawFrame(0)
    } else if (s.indexOf('interval') === 0) {
        interval = parseInt(s.split(' ')[1])
        console.log(`帧间隔时间设置为${interval}ms`)
    } else if (s.indexOf('play') === 0) {
        currentFrame = 0;
        timer = setInterval(() => {
            drawFrame(currentFrame)
            currentFrame++
            if (currentFrame === frameCount) {
                clearTimeout(timer)
            }
        }, interval)
    } else if (s.indexOf('pause') === 0) {
        clearTimeout(timer)
    } else if (s.indexOf('resume') === 0) {
        timer = setInterval(() => {
            drawFrame(currentFrame)
            currentFrame++
            if (currentFrame === frameCount) {
                clearTimeout(timer)
            }
        }, interval)
    } else {
        server_process.stdin.write(s)
    }
})

function drawFrame(i) {
    fs.readFile(`../functions_player/frame${i}.mcfunction`, async (err, data) => {
        server_process.stdin.write(data)
    })
}