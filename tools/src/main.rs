#![feature(iter_array_chunks)]
use anyhow::Result;
use clap::Parser;
use image::io::Reader as ImageReader;
use std::{
    fs::File,
    io::Write,
    mem::size_of,
    path::{Path, PathBuf},
    ptr, slice,
};
// BITMAP32
const MAGIC: u64 = 0x4249_544d_4150_3332;

// 16 bytes
#[repr(C)]
#[derive(Copy, Clone, PartialEq, Eq, Debug)]
struct Header {
    magic: u64,
    width: u32,
    height: u32,
}

impl Header {
    fn from_width_height(width: u32, height: u32) -> Self {
        assert!(width > 0);
        assert!(height > 0);
        Self {
            magic: MAGIC,
            width,
            height,
        }
    }
}

#[derive(Parser)]
#[command(version, about, long_about = None)]
/// Convert PNG file into a RGBA bitmap
///
/// (this is not a Windows .BMP conversion tool)
struct Args {
    /// The png file
    input: PathBuf,

    /// The output file
    #[arg(short, long, value_name = "FILE")]
    output: Option<PathBuf>,
}

fn main() -> Result<()> {
    let args = Args::parse();
    let input = args.input;
    let output = args.output.unwrap_or_else(|| {
        let mut output = input.clone();
        output.set_extension("rgba");
        output
    });

    let png = read_png(&input)?;
    write_bytes(&output, png.0, &png.1)
}

fn write_bytes(path: &Path, header: Header, bytes: &[u8]) -> Result<()> {
    assert_eq!(
        bytes.len() / size_of::<u32>(),
        (header.width * header.height).try_into()?
    );
    let mut file = File::create(path)?;
    let header = &header;
    file.write_all(unsafe {
        slice::from_raw_parts(ptr::from_ref(header) as *const u8, size_of::<Header>())
    })?;
    file.write_all(bytes)?;
    Ok(())
}

fn read_png(path: &Path) -> Result<(Header, Vec<u8>)> {
    let img = ImageReader::open(path)?.decode()?;
    let width = img.width();
    let height = img.height();
    let header = Header::from_width_height(width, height);

    let mut bytes = Vec::new();
    for pixel in img.into_rgba8().pixels() {
        for byte in pixel.0 {
            bytes.push(byte);
        }
    }

    assert_eq!(bytes.len() / 4, (width * height).try_into()?);

    Ok((header, bytes))
}

/*
fn read_png(path: &Path) -> Result<(Header,Vec<u8>)> {
    // The decoder is a build for reader and can be used to set various decoding options
    // via `Transformations`. The default output transformation is `Transformations::IDENTITY`.
    let decoder = png::Decoder::new(File::open(path)?);
    let mut reader = decoder.read_info()?;
    // Allocate the output buffer.
    let mut buf = vec![0; reader.output_buffer_size()];
    // Read the next frame. An APNG might contain multiple frames.
    let info = reader.next_frame(&mut buf)?;
    assert_eq!(info.bit_depth, png::BitDepth::Eight);
    assert!(reader.next_frame(&mut buf).is_err());
    // Grab the bytes of the image.
    Ok((Header::from_width_height(info.width, info.height), buf.to_vec()))
}
*/
