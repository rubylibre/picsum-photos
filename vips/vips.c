#include "vips.h"

int saveImageToBuffer(char *operation_name, VipsImage *image, void **buf, size_t *len) {
	VipsArea *area = NULL;
	int result;

	// Progressive, strip metadata
	result = vips_call(operation_name, image, &area, "interlace", TRUE, "strip", TRUE, "optimize_coding", TRUE, NULL);

	if (!result && area) {
		*buf = area->data;
		area->free_fn = NULL;
		*len = area->length;
		vips_area_unref(area);
	}

	return result;
}

int process_image(void *buf, size_t len, VipsImage **out) {
	VipsBlob *blob;
	int result;

	blob = vips_blob_new(NULL, buf, len);

	result = vips_call("thumbnail_buffer", blob, out, 100, "height", 100, NULL);

	vips_area_unref(VIPS_AREA(blob));

	return result;
}
