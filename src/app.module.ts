import { Module } from '@nestjs/common';
import { FirmwareModule } from './firmware/firmware.module';

@Module({
  imports: [FirmwareModule],
})
export class AppModule {}
