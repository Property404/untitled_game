// from https://stackoverflow.com/questions/51869520/image-to-uint8array-in-javascript
async function imageToUint8Array(image) {
    console.log(image);
    const canvas = document.querySelector("canvas")
    const context = canvas.getContext("2d");
    context.width = image.width;
    context.height = image.height;
    context.drawImage(image, 0, 0);
    const blob = await context.canvas.toBlob(
        callback,
        "image/jpeg", // the MIME type of the image format
        1 // quality (in the range 0..1)
    );
    return new Uint8Array(await blob.arrayBuffer());
}
