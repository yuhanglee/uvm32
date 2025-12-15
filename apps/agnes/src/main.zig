const uvm = @import("uvm.zig");
const zeptolibc = @import("zeptolibc");
const std = @import("std");

const console = @import("console.zig").getWriter();
const sdlkeys = @cImport({
    @cInclude("SDL_scancode.h");
});

const agnes = @cImport({
    @cInclude("agnes.h");
});

var ag: ?*agnes.agnes_t = null;
var ag_input: agnes.agnes_input_t = undefined;

const romData = @embedFile("croom.nes");

const WIDTH = 256;
const HEIGHT = 240;

var gfxFramebuffer: [WIDTH * HEIGHT]u32 = undefined;

fn consoleWriteFn(data:[]const u8) void {
    _ = console.print("{s}", .{data}) catch 0;
    _ = console.flush() catch 0;
}

fn checkKeys() void {
    var pressed:bool = undefined;
    var scancode:u16 = undefined;
    if (uvm.getkey(&scancode, &pressed)) {
        if (pressed) {
            switch(scancode) {
                sdlkeys.SDL_SCANCODE_RIGHT => ag_input.right = true,
                sdlkeys.SDL_SCANCODE_LEFT => ag_input.left = true,
                sdlkeys.SDL_SCANCODE_UP => ag_input.up = true,
                sdlkeys.SDL_SCANCODE_DOWN => ag_input.down = true,
                sdlkeys.SDL_SCANCODE_Z => ag_input.a = true,
                sdlkeys.SDL_SCANCODE_X => ag_input.b = true,
                sdlkeys.SDL_SCANCODE_TAB => ag_input.select = true,
                sdlkeys.SDL_SCANCODE_RETURN => ag_input.start = true,
                else => {},
            }
        } else {
            switch(scancode) {
                sdlkeys.SDL_SCANCODE_RIGHT => ag_input.right = false,
                sdlkeys.SDL_SCANCODE_LEFT => ag_input.left = false,
                sdlkeys.SDL_SCANCODE_UP => ag_input.up = false,
                sdlkeys.SDL_SCANCODE_DOWN => ag_input.down = false,
                sdlkeys.SDL_SCANCODE_Z => ag_input.a = false,
                sdlkeys.SDL_SCANCODE_X => ag_input.b = false,
                sdlkeys.SDL_SCANCODE_TAB => ag_input.select = false,
                sdlkeys.SDL_SCANCODE_RETURN => ag_input.start = false,
                else => {},
            }
        }
    }
    agnes.agnes_set_input(ag, &ag_input, 0);
}


fn submain() !void {
    // init zepto with a memory allocator and console writer
    zeptolibc.init(uvm.allocator(), consoleWriteFn);

    ag = agnes.agnes_make();
    if (agnes.agnes_load_ines_data(ag, @ptrCast(romData), romData.len)) {
        try console.print("load rom ok\n", .{});
    } else {
        try console.print("load rom failed\n", .{});
    }
    try console.flush();
    
    agnes.agnes_set_input(ag, &ag_input, 0);

    while(true) {
        if (!agnes.agnes_next_frame(ag)) {
            try console.print("Next frame failed!\n", .{});
            try console.flush();
        }

        var i:usize = 0;
        for (0..agnes.AGNES_SCREEN_HEIGHT) |y| {
            for (0..agnes.AGNES_SCREEN_WIDTH) |x| {
                const c = agnes.agnes_get_screen_pixel(ag, @intCast(x), @intCast(y));
                const c_val = @as(u32, @intCast(0xFF)) << 24 | @as(u32, @intCast(c.b)) << 16 | @as(u32, @intCast(c.g)) << 8 | @as(u32, @intCast(c.r));
                gfxFramebuffer[i] = c_val;
                i+=1;
            }
        }

        checkKeys();

        uvm.render(@ptrCast(&gfxFramebuffer), WIDTH * HEIGHT * 4);
    }
}

export fn main() void {
    _ = submain() catch {
        uvm.println("Caught err");
    };
}
